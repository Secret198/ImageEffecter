#ifndef COMPUTERENDERER_H
#define COMPUTERENDERER_H

#include <QObject>
#include <QQmlEngine>
#include <QQuickRhiItem>
#include <rhi/qrhi.h>
#include <QFile>
#include <QMatrix4x4>
#include <QQmlFile>
#include <vector>


class ComputeRenderer : public QQuickRhiItemRenderer
{

public:
    void initialize(QRhiCommandBuffer *cb) override;
    void synchronize(QQuickRhiItem *item) override;
    void render(QRhiCommandBuffer *cb) override;


private:
    void buildResources(QRhiResourceUpdateBatch *u);
    // QQuickRhiItemRenderer *createRenderer();
    void loadComputeShaders();
    void createComputePipeline();
    void createQuadPipeline();

    QRhi *m_rhi = nullptr;
    QImage m_image;
    bool m_newImage = false;
    bool m_haveResult = false;
    QRhiResourceUpdateBatch *m_pendingUpdates = nullptr;
    std::unique_ptr<QRhiBuffer> m_vbuf;
    std::unique_ptr<QRhiSampler> m_sampler;
    std::unique_ptr<QRhiTexture> m_inTex, m_outTex;
    std::unique_ptr<QRhiShaderResourceBindings> m_csSrbA, m_csSrbB, m_gfxSrb;
    std::vector<std::unique_ptr<QRhiComputePipeline>> m_csPipelines;
    std::unique_ptr<QRhiGraphicsPipeline> m_ps;

    std::vector<QShader> m_shaders;
    bool m_passSwitch = true;
};

#endif // COMPUTERENDERER_H
