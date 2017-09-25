//
//  gles2_info.c
//
//  Minimal code to dump EGL and GLESv2 info. Includes support for
//  Raspberry Pi.
//
//  Copyright 2017 Philip Lamb. All Rights Reserved.
//
//  Author(s): Philip Lamb
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// 
//      http://www.apache.org/licenses/LICENSE-2.0
// 
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include <stdio.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#ifdef RASPBERRY_PI
#  include <bcm_host.h>
#endif
 
int main()
{
    EGLDisplay m_display;
    EGLContext m_context;
    EGLConfig  m_config;
    EGLSurface m_surface;
    EGLBoolean result;

#ifdef RASPBERRY_PI
    atexit(bcm_host_deinit);   
    bcm_host_init();
#endif

    m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (m_display == EGL_NO_DISPLAY) {
        fprintf(stderr, "Error eglGetDisplay.\n");
        exit(EXIT_FAILURE);
    }
    
    int major, minor;
    result = eglInitialize(m_display, &major, &minor);
    if (result == EGL_FALSE) {
        fprintf(stderr, "Error eglInitialize.\n");
        exit(EXIT_FAILURE);
    }
    printf("EGL version %d.%d.\n", major, minor);

    result = eglBindAPI(EGL_OPENGL_ES_API);
    if (result == EGL_FALSE) {
        fprintf(stderr, "Error eglBindAPI.\n");
        exit(EXIT_FAILURE);
    }

    EGLint const attribs[] = {EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_CONFORMANT, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    EGLint numConfig;
    result = eglChooseConfig(m_display, attribs, &m_config, 1, &numConfig);
    if (result == EGL_FALSE) {
        fprintf(stderr, "Error eglChooseConfig.\n");
        exit(EXIT_FAILURE);
    }

    static const EGLint contextAttributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    m_context = eglCreateContext(m_display, m_config, EGL_NO_CONTEXT, contextAttributes);
    if (m_context == EGL_NO_CONTEXT) {
        fprintf(stderr, "Error eglCreateContext.\n");
        exit(EXIT_FAILURE);
    }
    
    EGLint const surfaceAttributes[] = {
        EGL_NONE
    };
    m_surface = eglCreatePbufferSurface(m_display, m_config, surfaceAttributes);
    if (m_surface == EGL_NO_SURFACE) {
        fprintf(stderr, "Error eglCreatePbufferSurface.\n");
        exit(EXIT_FAILURE);
    }
    
    result = eglMakeCurrent(m_display, m_surface, m_surface, m_context);
    if (result == EGL_FALSE) {
        fprintf(stderr, "Error eglMakeCurrent.\n");
        exit(EXIT_FAILURE);
    }
    
    const unsigned char *strVendor = glGetString(GL_VENDOR);
    printf("GL_VENDOR=%s\n", strVendor);
    const unsigned char *strRenderer = glGetString(GL_RENDERER);
    printf("GL_RENDERER=%s\n", strRenderer);
    const unsigned char *strVersion = glGetString(GL_VERSION);
    printf("GL_VERSION=%s\n", strVersion);
    const unsigned char *strExtensions = glGetString(GL_EXTENSIONS);
    printf("GL_EXTENSIONS=%s\n", strExtensions);

    eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(m_display, m_surface);
    eglDestroyContext(m_display, m_context);
    eglTerminate(m_display);
    
    return 0;
}
