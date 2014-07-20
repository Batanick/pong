#include "ReflectionHolder.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "TexturedFrame.h"

#include "logging.h"

void ReflectionHolder::onInit() {
  glGenTextures(1, &textureId);
  ASSERT((textureId != GL_INVALID_VALUE), "Unable to initialize texture");

  glGenFramebuffers(1, &frameBuffer);
  ASSERT((frameBuffer != GL_INVALID_VALUE), "Unable to init frame buffer");

  glGenRenderbuffers(1, &depthBuffer);
  ASSERT((depthBuffer != GL_INVALID_VALUE), "Unable to init depth buffer");
}

void ReflectionHolder::onContextInit(RenderContext &context) {
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, context.windowWidth, context.windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureId, 0);

  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, context.windowWidth, context.windowHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

  ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Unable to init frameBuffer status");

  context.reflectionTexId = textureId;
}

void ReflectionHolder::onBeforeRender(const RenderContext &context) {
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



  renderer.render(CommonRenderer::Common, context);
}

void ReflectionHolder::onInitScene() {
  renderer.add(std::shared_ptr<TexturedFrame>(new TexturedFrame(0.5f, -1.0f, 0.5f, 0.5f, textureId)), CommonRenderer::GUI);
}


void ReflectionHolder::onShutdown() {
  glDeleteTextures(1, &textureId);
  glDeleteRenderbuffers(1, &depthBuffer);
  glDeleteFramebuffers(1, &frameBuffer);
}