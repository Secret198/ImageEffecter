#include "computerenderer.h"
#include "computeitem.h"


static QShader loadQsb(const QString &name)
{
    QFile f(name);
    if (!f.open(QIODevice::ReadOnly))
        qWarning("Cannot open %s", qPrintable(name));
    return QShader::fromSerialized(f.readAll());
}




QQuickRhiItemRenderer *ComputeItem::createRenderer()
{
    return new ComputeRenderer;
}


void ComputeRenderer::initialize(QRhiCommandBuffer *cb){
    if (m_rhi != rhi()) {               // first time, or RHI was recreated
        m_ps.reset();
        m_csPipelines.clear();
        m_csSrbA.reset(); m_csSrbB.reset();
        m_gfxSrb.reset();
        m_inTex.reset(); m_outTex.reset();
        m_vbuf.reset(); m_sampler.reset();
        m_rhi = rhi();
        m_newImage = !m_image.isNull(); // force rebuild if we already have an image
        m_haveResult = false;
    }
    if (m_vbuf) return;                 // already initialized

    m_sampler.reset(m_rhi->newSampler(QRhiSampler::Linear, QRhiSampler::Linear,
                                      QRhiSampler::None, QRhiSampler::ClampToEdge, QRhiSampler::ClampToEdge));
    m_sampler->create();

    static const float verts[] = {
        -1,-1, 0,1,   1,-1, 1,1,   -1, 1, 0,0,   1, 1, 1,0
    };
    m_vbuf.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                  QRhiBuffer::VertexBuffer, sizeof(verts)));
    m_vbuf->create();

    m_pendingUpdates = m_rhi->nextResourceUpdateBatch();
    m_pendingUpdates->uploadStaticBuffer(m_vbuf.get(), verts);
    m_shaders.clear();
    loadComputeShaders();
}

void ComputeRenderer::synchronize(QQuickRhiItem *item)
{
    auto *it = static_cast<ComputeItem *>(item);
    if (it->m_dirty) {
        m_image = it->img();
        m_newImage = true;
        it->m_dirty = false;
    }
}

void ComputeRenderer::render(QRhiCommandBuffer *cb){
    QRhiResourceUpdateBatch *u = m_pendingUpdates ? m_pendingUpdates : m_rhi->nextResourceUpdateBatch();
    m_pendingUpdates = nullptr;

    if (m_newImage && !m_image.isNull()) {
        buildResources(u);          // (re)create textures, SRBs, pipelines
        m_newImage = false;
        m_haveResult = true;
    }
    if (!m_haveResult) {            // nothing to show yet
        cb->beginPass(renderTarget(), Qt::transparent, {1.0f, 0}, u);
        cb->endPass();
        return;
    }

    // 1) compute
    for(int i = 0;i<m_csPipelines.size();i++){
        cb->beginComputePass((i == 0) ? u : nullptr);
        cb->setComputePipeline(m_csPipelines[i].get());
        cb->setShaderResources(m_passSwitch ? m_csSrbA.get() : m_csSrbB.get());
        const QSize s = m_image.size();
        cb->dispatch((s.width() + 15) / 16, (s.height() + 15) / 16, 1);
        cb->endComputePass();
        m_passSwitch = !m_passSwitch;
    }

    // 2) draw quad (QRhi inserts the needed barrier between the passes)
    cb->beginPass(renderTarget(), Qt::transparent, {1.0f, 0});
    cb->setGraphicsPipeline(m_ps.get());

    const QSize out = renderTarget()->pixelSize();
    const QSize img = m_image.size();
    const float scale = qMin(float(out.width()) / img.width(), float(out.height()) / img.height());
    const float w = img.width()  * scale;
    const float h = img.height() * scale;
    const float x = (out.width()  - w) * 0.5f;
    const float y = (out.height() - h) * 0.5f;

    cb->setViewport({x, y, w, h});
    cb->setShaderResources(m_gfxSrb.get());
    const QRhiCommandBuffer::VertexInput vi(m_vbuf.get(), 0);
    cb->setVertexInput(0, 1, &vi);
    cb->draw(4);
    cb->endPass();
}

void ComputeRenderer::loadComputeShaders(){
    m_shaders.push_back(loadQsb(":/shaders/test_compute.comp.qsb"));
    m_shaders.push_back(loadQsb(":/shaders/test_compute2.comp.qsb"));
}

void ComputeRenderer::createComputePipeline(){
    m_csSrbA.reset(m_rhi->newShaderResourceBindings());
    m_csSrbA->setBindings({
        QRhiShaderResourceBinding::sampledTexture(0, QRhiShaderResourceBinding::ComputeStage, m_inTex.get(), m_sampler.get()),
        QRhiShaderResourceBinding::imageStore(1, QRhiShaderResourceBinding::ComputeStage, m_outTex.get(), 0)
    });
    m_csSrbA->create();

    m_csSrbB.reset(m_rhi->newShaderResourceBindings());
    m_csSrbB->setBindings({
        QRhiShaderResourceBinding::sampledTexture(0, QRhiShaderResourceBinding::ComputeStage, m_outTex.get(), m_sampler.get()),
        QRhiShaderResourceBinding::imageStore(1, QRhiShaderResourceBinding::ComputeStage, m_inTex.get(), 0)
    });
    m_csSrbB->create();

    m_csPipelines.clear();
    m_csPipelines.resize(m_shaders.size());
    for(int i = 0;i<m_shaders.size();i++){

        m_csPipelines[i].reset(m_rhi->newComputePipeline());
        m_csPipelines[i]->setShaderStage({QRhiShaderStage::Compute, m_shaders[i]});
        m_csPipelines[i]->setShaderResourceBindings((m_passSwitch) ? m_csSrbA.get() : m_csSrbB.get());
        m_csPipelines[i]->create();
    }

}

void ComputeRenderer::createQuadPipeline(){
    m_gfxSrb.reset(m_rhi->newShaderResourceBindings());
    m_gfxSrb->setBindings({
        QRhiShaderResourceBinding::sampledTexture(1, QRhiShaderResourceBinding::FragmentStage, m_passSwitch ? m_inTex.get() : m_outTex.get(), m_sampler.get())
    });
    m_gfxSrb->create();

    m_ps.reset(m_rhi->newGraphicsPipeline());
    m_ps->setTopology(QRhiGraphicsPipeline::TriangleStrip);
    m_ps->setShaderStages({
        {QRhiShaderStage::Vertex,   loadQsb(":/shaders/quad.vert.qsb")},
        {QRhiShaderStage::Fragment, loadQsb(":/shaders/quad.frag.qsb")}
    });
    QRhiVertexInputLayout layout;
    layout.setBindings({{4 * sizeof(float)}});
    layout.setAttributes({
        {0, 0, QRhiVertexInputAttribute::Float2, 0},
        {0, 1, QRhiVertexInputAttribute::Float2, 2 * sizeof(float)}
    });
    m_ps->setVertexInputLayout(layout);
    m_ps->setShaderResourceBindings(m_gfxSrb.get());
    m_ps->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    m_ps->create();
}

void ComputeRenderer::buildResources(QRhiResourceUpdateBatch *u){

    const QSize sz = m_image.size();

    m_inTex.reset(m_rhi->newTexture(QRhiTexture::RGBA8, sz, 1, QRhiTexture::UsedWithLoadStore));
    m_inTex->create();
    u->uploadTexture(m_inTex.get(), m_image);

    m_outTex.reset(m_rhi->newTexture(QRhiTexture::RGBA8, sz, 1, QRhiTexture::UsedWithLoadStore));
    m_outTex->create();

    // Compute
    createComputePipeline();

    // Graphics
    createQuadPipeline();
}