/* San Angeles Observation OpenGL ES version example
 * Copyright 2004-2005 Jetro Lauha
 * All rights reserved.
 * Web: http://iki.fi/jetro/
 *
 * This source is free software; you can redistribute it and/or
 * modify it under the terms of EITHER:
 *   (1) The GNU Lesser General Public License as published by the Free
 *       Software Foundation; either version 2.1 of the License, or (at
 *       your option) any later version. The text of the GNU Lesser
 *       General Public License is included with this source in the
 *       file LICENSE-LGPL.txt.
 *   (2) The BSD-style license that is included with this source in
 *       the file LICENSE-BSD.txt.
 *
 * This source is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files
 * LICENSE-LGPL.txt and LICENSE-BSD.txt for more details.
 *
 * $Id: importgl.c,v 1.4 2005/02/08 18:42:55 tonic Exp $
 * $Revision: 1.4 $
 */

#undef WIN32
#undef LINUX
#ifdef _MSC_VER
// Desktop or mobile Win32 environment:
#define WIN32
#else
// Linux environment:
#define LINUX
#endif

#ifndef DISABLE_IMPORTGL

#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
static HMODULE sGLESDLL = NULL;
#endif // WIN32

#ifdef LINUX
#include <stdlib.h>
#include <dlfcn.h>
static void *sGLESSO = NULL;
#endif // LINUX

#endif /* DISABLE_IMPORTGL */

#define IMPORTGL_NO_FNPTR_DEFS
#define IMPORTGL_API
#define IMPORTGL_FNPTRINIT = NULL
#include "importgl.h"


/* Imports function pointers to selected function calls in OpenGL ES Common
 * or Common Lite profile DLL or shared object. The function pointers are
 * stored as global symbols with equivalent function name but prefixed with
 * "funcPtr_". Standard gl/egl calls are redirected to the function pointers
 * with preprocessor macros (see importgl.h).
 */
int importGLInit()
{
    int result = 1;

#ifndef DISABLE_IMPORTGL

#undef IMPORT_FUNC

#ifdef WIN32
    sGLESDLL = LoadLibrary(_T("libGLES_CM.dll"));
    if (sGLESDLL == NULL)
        sGLESDLL = LoadLibrary(_T("libGLES_CL.dll"));
    if (sGLESDLL == NULL)
        return 0;   // Cannot find OpenGL ES Common or Common Lite DLL.

    /* The following fetches address to each egl & gl function call
     * and stores it to the related function pointer. Casting through
     * void * results in warnings with VC warning level 4, which
     * could be fixed by casting to the true type for each fetch.
     */
#define IMPORT_FUNC(funcName) do { \
        void *procAddress = (void *)GetProcAddress(sGLESDLL, _T(#funcName)); \
        if (procAddress == NULL) result = 0; \
        *((void **)&FNPTR(funcName)) = procAddress; } while (0)
#endif // WIN32

#ifdef LINUX
#ifdef ANDROID_NDK
    sGLESSO = dlopen("libGLESv1_CM.so", RTLD_NOW); //ghd214
//    sGLESSO = dlopen("libGLESv2.so", RTLD_NOW); //ghd214
      
#else /* !ANDROID_NDK */
    sGLESSO = dlopen("libGLES_CM.so", RTLD_NOW);
    if (sGLESSO == NULL)
        sGLESSO = dlopen("libGLES_CL.so", RTLD_NOW);
#endif /* !ANDROID_NDK */
    if (sGLESSO == NULL)
        return 0;   // Cannot find OpenGL ES Common or Common Lite SO.

#define IMPORT_FUNC(funcName) do { \
        void *procAddress = (void *)dlsym(sGLESSO, #funcName); \
        if (procAddress == NULL) result = 0; \
        *((void **)&FNPTR(funcName)) = procAddress; } while (0)
#endif // LINUX

#ifndef ANDROID_NDK
    IMPORT_FUNC(eglChooseConfig);
    IMPORT_FUNC(eglCreateContext);
    IMPORT_FUNC(eglCreateWindowSurface);
    IMPORT_FUNC(eglDestroyContext);
    IMPORT_FUNC(eglDestroySurface);
    IMPORT_FUNC(eglGetConfigAttrib);
    IMPORT_FUNC(eglGetConfigs);
    IMPORT_FUNC(eglGetDisplay);
    IMPORT_FUNC(eglGetError);
    IMPORT_FUNC(eglInitialize);
    IMPORT_FUNC(eglMakeCurrent);
    IMPORT_FUNC(eglSwapBuffers);
    IMPORT_FUNC(eglTerminate);
#endif /* !ANDROID_NDK */

    IMPORT_FUNC(glBlendFunc);
    IMPORT_FUNC(glClear);
    IMPORT_FUNC(glClearColorx);
    IMPORT_FUNC(glColor4x);
    IMPORT_FUNC(glColorPointer);
    IMPORT_FUNC(glDisable);
    IMPORT_FUNC(glDisableClientState);
    IMPORT_FUNC(glDrawArrays);
    IMPORT_FUNC(glEnable);
    IMPORT_FUNC(glEnableClientState);
    IMPORT_FUNC(glFrustumx);
    IMPORT_FUNC(glGetError);
    IMPORT_FUNC(glLightxv);
    IMPORT_FUNC(glLoadIdentity);
    IMPORT_FUNC(glMaterialx);
    IMPORT_FUNC(glMaterialxv);
    IMPORT_FUNC(glMatrixMode);
    IMPORT_FUNC(glMultMatrixx);
    IMPORT_FUNC(glNormalPointer);
    IMPORT_FUNC(glPopMatrix);
    IMPORT_FUNC(glPushMatrix);
    IMPORT_FUNC(glRotatex);
    IMPORT_FUNC(glScalex);
    IMPORT_FUNC(glShadeModel);
    IMPORT_FUNC(glTranslatex);
    IMPORT_FUNC(glVertexPointer);
    IMPORT_FUNC(glViewport);

    //ghd
   IMPORT_FUNC(glActiveTexture);
            IMPORT_FUNC(glAttachShader);
            IMPORT_FUNC(glBindAttribLocation);
            IMPORT_FUNC(glBindBuffer);
            IMPORT_FUNC(glBindFramebuffer);
            IMPORT_FUNC(glBindRenderbuffer);
            IMPORT_FUNC(glBindTexture);
            IMPORT_FUNC(glBlendColor);
            IMPORT_FUNC(glBlendEquation);
            IMPORT_FUNC(glBlendEquationSeparate);
            IMPORT_FUNC(glBlendFuncSeparate);
            IMPORT_FUNC(glBufferData);
            IMPORT_FUNC(glBufferSubData);
            IMPORT_FUNC(glCheckFramebufferStatus);
            IMPORT_FUNC(glClear);
            IMPORT_FUNC(glClearColor);
            IMPORT_FUNC(glClearDepthf);
            IMPORT_FUNC(glClearStencil);
            IMPORT_FUNC(glColorMask);
            IMPORT_FUNC(glCompileShader);
            IMPORT_FUNC(glCompressedTexImage2D);
            IMPORT_FUNC(glCompressedTexSubImage2D);
           IMPORT_FUNC(glCopyTexImage2D);
           IMPORT_FUNC(glCopyTexSubImage2D);
           IMPORT_FUNC(glCreateProgram);
           IMPORT_FUNC(glCreateShader);
           IMPORT_FUNC(glCullFace);
           IMPORT_FUNC(glDeleteBuffers);
           IMPORT_FUNC(glDeleteFramebuffers);
           IMPORT_FUNC(glDeleteProgram);
           IMPORT_FUNC(glDeleteRenderbuffers);
           IMPORT_FUNC(glDeleteShader);
           IMPORT_FUNC(glDeleteTextures);
           IMPORT_FUNC(glDepthFunc);
           IMPORT_FUNC(glDepthMask);
           IMPORT_FUNC(glDepthRangef);
           IMPORT_FUNC(glDetachShader);
           IMPORT_FUNC(glDisable);
           IMPORT_FUNC(glDisableVertexAttribArray);
           IMPORT_FUNC(glDrawArrays);
           IMPORT_FUNC(glDrawElements);
           IMPORT_FUNC(glEnable);
           IMPORT_FUNC(glEnableVertexAttribArray);
           IMPORT_FUNC(glFinish);
           IMPORT_FUNC(glFlush);
           IMPORT_FUNC(glFramebufferRenderbuffer);
           IMPORT_FUNC(glFramebufferTexture2D);
           IMPORT_FUNC(glFrontFace);
           IMPORT_FUNC(glGenBuffers);
           IMPORT_FUNC(glGenerateMipmap);
           IMPORT_FUNC(glGenFramebuffers);
           IMPORT_FUNC(glGenRenderbuffers);
           IMPORT_FUNC(glGenTextures);
           IMPORT_FUNC(glGetActiveAttrib);
           IMPORT_FUNC(glGetActiveUniform);
           IMPORT_FUNC(glGetAttachedShaders);
           IMPORT_FUNC(glGetAttribLocation);
           IMPORT_FUNC(glGetBooleanv);
           IMPORT_FUNC(glGetBufferParameteriv);
           IMPORT_FUNC(glGetError);
           IMPORT_FUNC(glGetFloatv);
           IMPORT_FUNC(glGetFramebufferAttachmentParameteriv);
           IMPORT_FUNC(glGetIntegerv);
           IMPORT_FUNC(glGetProgramiv);
           IMPORT_FUNC(glGetProgramInfoLog);
           IMPORT_FUNC(glGetRenderbufferParameteriv);
           IMPORT_FUNC(glGetShaderiv);
           IMPORT_FUNC(glGetShaderInfoLog);
           IMPORT_FUNC(glGetShaderPrecisionFormat);
           IMPORT_FUNC(glGetShaderSource);
     		  IMPORT_FUNC(glGetString);
            IMPORT_FUNC(glGetTexParameterfv);
            IMPORT_FUNC(glGetTexParameteriv);
            IMPORT_FUNC(glGetUniformfv);
            IMPORT_FUNC(glGetUniformiv);
            IMPORT_FUNC(glGetUniformLocation);
            IMPORT_FUNC(glGetVertexAttribfv);
            IMPORT_FUNC(glGetVertexAttribiv);
            IMPORT_FUNC(glGetVertexAttribPointerv);
            IMPORT_FUNC(glHint);
       	  IMPORT_FUNC(glIsBuffer);
        	  IMPORT_FUNC(glIsEnabled);
       	  IMPORT_FUNC(glIsFramebuffer);
        	  IMPORT_FUNC(glIsProgram);
       	  IMPORT_FUNC(glIsRenderbuffer);
        	  IMPORT_FUNC(glIsShader);
        	  IMPORT_FUNC(glIsTexture);
            IMPORT_FUNC(glLineWidth);
            IMPORT_FUNC(glLinkProgram);
            IMPORT_FUNC(glPixelStorei);
            IMPORT_FUNC(glPolygonOffset);
            IMPORT_FUNC(glReadPixels);
            IMPORT_FUNC(glReleaseShaderCompiler);
            IMPORT_FUNC(glRenderbufferStorage);
            IMPORT_FUNC(glSampleCoverage);
            IMPORT_FUNC(glScissor);
            IMPORT_FUNC(glShaderBinary);
            IMPORT_FUNC(glShaderSource);
            IMPORT_FUNC(glStencilFunc);
            IMPORT_FUNC(glStencilFuncSeparate);
            IMPORT_FUNC(glStencilMask);
            IMPORT_FUNC(glStencilMaskSeparate);
            IMPORT_FUNC(glStencilOp);
            IMPORT_FUNC(glStencilOpSeparate);
            IMPORT_FUNC(glTexImage2D);
            IMPORT_FUNC(glTexParameterf);
            IMPORT_FUNC(glTexParameterfv);
            IMPORT_FUNC(glTexParameteri);
            IMPORT_FUNC(glTexParameteriv);
            IMPORT_FUNC(glTexSubImage2D);
            IMPORT_FUNC(glUniform1f);
            IMPORT_FUNC(glUniform1fv);
            IMPORT_FUNC(glUniform1i);
            IMPORT_FUNC(glUniform1iv);
            IMPORT_FUNC(glUniform3f);
            IMPORT_FUNC(glUniform3fv);
            IMPORT_FUNC(glUniform3i);
            IMPORT_FUNC(glUniform3iv);
            IMPORT_FUNC(glUniform4f);
            IMPORT_FUNC(glUniform4fv);
            IMPORT_FUNC(glUniform4i);
            IMPORT_FUNC(glUniform4iv);
            IMPORT_FUNC(glUniformMatrix2fv);
            IMPORT_FUNC(glUniformMatrix3fv);
            IMPORT_FUNC(glUniformMatrix4fv);
            IMPORT_FUNC(glUseProgram);
            IMPORT_FUNC(glValidateProgram);
            IMPORT_FUNC(glVertexAttrib1f);
            IMPORT_FUNC(glVertexAttrib1fv);
            IMPORT_FUNC(glVertexAttrib2f);
            IMPORT_FUNC(glVertexAttrib2fv);
            IMPORT_FUNC(glVertexAttrib3f);
            IMPORT_FUNC(glVertexAttrib3fv);
            IMPORT_FUNC(glVertexAttrib4f);
            IMPORT_FUNC(glVertexAttrib4fv);
            IMPORT_FUNC(glVertexAttribPointer);
            IMPORT_FUNC(glViewport);

    //ghd


#endif /* DISABLE_IMPORTGL */

    return result;
}


void importGLDeinit()
{
#ifndef DISABLE_IMPORTGL
#ifdef WIN32
    FreeLibrary(sGLESDLL);
#endif

#ifdef LINUX
    dlclose(sGLESSO);
#endif
#endif /* DISABLE_IMPORTGL */
}
