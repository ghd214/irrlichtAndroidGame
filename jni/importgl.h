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
 * $Id: importgl.h,v 1.4 2005/02/24 20:29:33 tonic Exp $
 * $Revision: 1.4 $
 */

#ifndef IMPORTGL_H_INCLUDED
#define IMPORTGL_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif


//#include <GLES2/gl2.h> //ghd214
//#include <GLES2/gl2ext.h> //ghd214

#include <GLES/gl.h>
#include <GLES/glext.h> 

#ifndef ANDROID_NDK
#include <GLES/egl.h>
#endif /* !ANDROID_NDK */

/* Use DISABLE_IMPORTGL if you want to link the OpenGL ES at
 * compile/link time and not import it dynamically runtime.
 */
#ifndef DISABLE_IMPORTGL


/* Dynamically fetches pointers to the egl & gl functions.
 * Should be called once on application initialization.
 * Returns non-zero on success and 0 on failure.
 */
extern int importGLInit();

/* Frees the handle to egl & gl functions library.
 */
extern void importGLDeinit();


#ifndef IMPORTGL_API
#define IMPORTGL_API extern
#endif
#ifndef IMPORTGL_FNPTRINIT
#define IMPORTGL_FNPTRINIT
#endif

#define FNDEF(retType, funcName, args) IMPORTGL_API retType (*funcPtr_##funcName) args IMPORTGL_FNPTRINIT

#ifndef ANDROID_NDK
FNDEF(EGLBoolean, eglChooseConfig, (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config));
FNDEF(EGLContext, eglCreateContext, (EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint *attrib_list));
FNDEF(EGLSurface, eglCreateWindowSurface, (EGLDisplay dpy, EGLConfig config, NativeWindowType window, const EGLint *attrib_list));
FNDEF(EGLBoolean, eglDestroyContext, (EGLDisplay dpy, EGLContext ctx));
FNDEF(EGLBoolean, eglDestroySurface, (EGLDisplay dpy, EGLSurface surface));
FNDEF(EGLBoolean, eglGetConfigAttrib, (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value));
FNDEF(EGLBoolean, eglGetConfigs, (EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config));
FNDEF(EGLDisplay, eglGetDisplay, (NativeDisplayType display));
FNDEF(EGLint, eglGetError, (void));
FNDEF(EGLBoolean, eglInitialize, (EGLDisplay dpy, EGLint *major, EGLint *minor));
FNDEF(EGLBoolean, eglMakeCurrent, (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx));
FNDEF(EGLBoolean, eglSwapBuffers, (EGLDisplay dpy, EGLSurface draw));
FNDEF(EGLBoolean, eglTerminate, (EGLDisplay dpy));
#endif /* !ANDROID_NDK */

FNDEF(void, glBlendFunc, (GLenum sfactor, GLenum dfactor));
FNDEF(void, glClear, (GLbitfield mask));
FNDEF(void, glClearColorx, (GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha));
FNDEF(void, glColor4x, (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha));
FNDEF(void, glColorPointer, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glDisable, (GLenum cap));
FNDEF(void, glDisableClientState, (GLenum array));
FNDEF(void, glDrawArrays, (GLenum mode, GLint first, GLsizei count));
FNDEF(void, glEnable, (GLenum cap));
FNDEF(void, glEnableClientState, (GLenum array));
FNDEF(void, glFrustumx, (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar));
FNDEF(GLenum, glGetError, (void));
FNDEF(void, glLightxv, (GLenum light, GLenum pname, const GLfixed *params));
FNDEF(void, glLoadIdentity, (void));
FNDEF(void, glMaterialx, (GLenum face, GLenum pname, GLfixed param));
FNDEF(void, glMaterialxv, (GLenum face, GLenum pname, const GLfixed *params));
FNDEF(void, glMatrixMode, (GLenum mode));
FNDEF(void, glMultMatrixx, (const GLfixed *m));
FNDEF(void, glNormalPointer, (GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glPopMatrix, (void));
FNDEF(void, glPushMatrix, (void));
FNDEF(void, glRotatex, (GLfixed angle, GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glScalex, (GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glShadeModel, (GLenum mode));
FNDEF(void, glTranslatex, (GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glVertexPointer, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height));

//ghd{
FNDEF(void, glActiveTexture, (GLenum texture));
FNDEF(void,          glAttachShader, (GLuint program, GLuint shader));
FNDEF(void,          glBindAttribLocation, (GLuint program, GLuint index, const char* name));
FNDEF(void,          glBindBuffer, (GLenum target, GLuint buffer));
FNDEF(void,         glBindFramebuffer, (GLenum target, GLuint framebuffer));
FNDEF(void,          glBindRenderbuffer, (GLenum target, GLuint renderbuffer));
FNDEF(void,          glBindTexture, (GLenum target, GLuint texture));
FNDEF(void,          glBlendColor, (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha));
FNDEF(void,           glBlendEquation, ( GLenum mode ));
FNDEF(void,           glBlendEquationSeparate, (GLenum modeRGB, GLenum modeAlpha));
FNDEF(void,           glBlendFunc ,(GLenum sfactor, GLenum dfactor));
FNDEF( void,           glBlendFuncSeparate, (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha));
FNDEF(void ,          glBufferData, (GLenum target, GLsizeiptr size, const void* data, GLenum usage));
FNDEF(void,           glBufferSubData, (GLenum target, GLintptr offset, GLsizeiptr size, const void* data));
FNDEF(GLenum,         glCheckFramebufferStatus, (GLenum target));
FNDEF(void,           glClear ,(GLbitfield mask));
FNDEF(void,           glClearColor, (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha));
FNDEF(void,           glClearDepthf ,(GLclampf depth));
FNDEF(void,           glClearStencil, (GLint s));
FNDEF(void,           glColorMask ,(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha));
FNDEF(void,           glCompileShader, (GLuint shader));
FNDEF(void,           glCompressedTexImage2D, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data));
FNDEF( void,           glCompressedTexSubImage2D ,(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data));
FNDEF(void ,          glCopyTexImage2D ,(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border));
FNDEF( void,           glCopyTexSubImage2D ,(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height));
FNDEF( GLuint ,        glCreateProgram, (void));
FNDEF( GLuint  ,       glCreateShader, (GLenum type));
FNDEF( void,           glCullFace ,(GLenum mode));
FNDEF(void ,          glDeleteBuffers ,(GLsizei n, const GLuint* buffers));
FNDEF( void,           glDeleteFramebuffers, (GLsizei n, const GLuint* framebuffers));
FNDEF( void ,          glDeleteProgram ,(GLuint program));
FNDEF( void ,          glDeleteRenderbuffers, (GLsizei n, const GLuint* renderbuffers));
FNDEF( void ,          glDeleteShader ,(GLuint shader));
FNDEF( void ,          glDeleteTextures ,(GLsizei n, const GLuint* textures));
FNDEF( void ,          glDepthFunc ,(GLenum func));
FNDEF( void ,          glDepthMask ,(GLboolean flag));
FNDEF( void ,          glDepthRangef ,(GLclampf zNear, GLclampf zFar));
FNDEF( void ,          glDetachShader, (GLuint program, GLuint shader));
FNDEF( void ,          glDisable, (GLenum cap));
FNDEF( void ,          glDisableVertexAttribArray, (GLuint index));
FNDEF( void ,          glDrawArrays, (GLenum mode, GLint first, GLsizei count));
FNDEF( void ,          glDrawElements, (GLenum mode, GLsizei count, GLenum type, const void* indices));
FNDEF( void ,          glEnable ,(GLenum cap));
FNDEF( void ,          glEnableVertexAttribArray, (GLuint index));
FNDEF( void ,          glFinish,(void));
FNDEF( void ,          glFlush ,(void));
FNDEF( void ,          glFramebufferRenderbuffer, (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer));
FNDEF( void ,          glFramebufferTexture2D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level));
FNDEF( void ,          glFrontFace ,(GLenum mode));
FNDEF( void ,          glGenBuffers ,(GLsizei n, GLuint* buffers));
FNDEF( void ,          glGenerateMipmap, (GLenum target));
FNDEF( void ,          glGenFramebuffers, (GLsizei n, GLuint* framebuffers));
FNDEF( void ,          glGenRenderbuffers, (GLsizei n, GLuint* renderbuffers));
FNDEF( void ,          glGenTextures, (GLsizei n, GLuint* textures));
FNDEF( void ,          glGetActiveAttrib, (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name));
FNDEF( void ,          glGetActiveUniform, (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name));
FNDEF( void ,          glGetAttachedShaders, (GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders));
FNDEF( int  ,          glGetAttribLocation, (GLuint program, const char* name));
FNDEF( void ,          glGetBooleanv, (GLenum pname, GLboolean* params));
FNDEF( void ,          glGetBufferParameteriv, (GLenum target, GLenum pname, GLint* params));
FNDEF( GLenum,         glGetError, (void));
FNDEF( void ,          glGetFloatv ,(GLenum pname, GLfloat* params));
FNDEF( void ,          glGetFramebufferAttachmentParameteriv, (GLenum target, GLenum attachment, GLenum pname, GLint* params));
FNDEF( void ,          glGetIntegerv ,(GLenum pname, GLint* params));
FNDEF( void ,          glGetProgramiv, (GLuint program, GLenum pname, GLint* params));
FNDEF( void ,          glGetProgramInfoLog ,(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog));
FNDEF( void ,          glGetRenderbufferParameteriv, (GLenum target, GLenum pname, GLint* params));
FNDEF( void ,          glGetShaderiv ,(GLuint shader, GLenum pname, GLint* params));
FNDEF( void ,          glGetShaderInfoLog ,(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog));
FNDEF( void ,          glGetShaderPrecisionFormat, (GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision));
FNDEF( void ,          glGetShaderSource, (GLuint shader, GLsizei bufsize, GLsizei* length, char* source));
FNDEF( const GLubyte*,   glGetString, (GLenum name));
FNDEF( void,           glGetTexParameterfv, (GLenum target, GLenum pname, GLfloat* params));
FNDEF( void,           glGetTexParameteriv ,(GLenum target, GLenum pname, GLint* params));
FNDEF( void,           glGetUniformfv ,(GLuint program, GLint location, GLfloat* params));
FNDEF( void,           glGetUniformiv, (GLuint program, GLint location, GLint* params));
FNDEF( int,            glGetUniformLocation, (GLuint program, const char* name));
FNDEF( void,           glGetVertexAttribfv ,(GLuint index, GLenum pname, GLfloat* params));
FNDEF( void,           glGetVertexAttribiv, (GLuint index, GLenum pname, GLint* params));
FNDEF( void,           glGetVertexAttribPointerv, (GLuint index, GLenum pname, void** pointer));
FNDEF( void,           glHint ,(GLenum target, GLenum mode));
FNDEF( GLboolean,      glIsBuffer, (GLuint buffer));
FNDEF( GLboolean,      glIsEnabled ,(GLenum cap));
FNDEF( GLboolean,      glIsFramebuffer ,(GLuint framebuffer));
FNDEF( GLboolean,      glIsProgram, (GLuint program));
FNDEF( GLboolean,      glIsRenderbuffer, (GLuint renderbuffer));
FNDEF( GLboolean,      glIsShader ,(GLuint shader));
FNDEF( GLboolean,      glIsTexture, (GLuint texture));
FNDEF( void,           glLineWidth ,(GLfloat width));
FNDEF( void,           glLinkProgram, (GLuint program));
FNDEF( void,           glPixelStorei, (GLenum pname, GLint param));
FNDEF( void,           glPolygonOffset, (GLfloat factor, GLfloat units));
FNDEF( void,           glReadPixels ,(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels));
FNDEF( void,           glReleaseShaderCompiler, (void));
FNDEF( void,           glRenderbufferStorage ,(GLenum target, GLenum internalformat, GLsizei width, GLsizei height));
FNDEF( void,           glSampleCoverage ,(GLclampf value, GLboolean invert));
FNDEF( void,           glScissor ,(GLint x, GLint y, GLsizei width, GLsizei height));
FNDEF( void,           glShaderBinary ,(GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length));
FNDEF( void,           glShaderSource ,(GLuint shader, GLsizei count, const char** string, const GLint* length));
FNDEF( void,           glStencilFunc ,(GLenum func, GLint ref, GLuint mask));
FNDEF( void,           glStencilFuncSeparate ,(GLenum face, GLenum func, GLint ref, GLuint mask));
FNDEF( void,           glStencilMask ,(GLuint mask));
FNDEF( void,           glStencilMaskSeparate ,(GLenum face, GLuint mask));
FNDEF( void,           glStencilOp ,(GLenum fail, GLenum zfail, GLenum zpass));
FNDEF( void,           glStencilOpSeparate ,(GLenum face, GLenum fail, GLenum zfail, GLenum zpass));
FNDEF( void,           glTexImage2D ,(GLenum target, GLint level, GLint internalformat,  GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels));
FNDEF( void,           glTexParameterf,(GLenum target, GLenum pname, GLfloat param));
FNDEF( void,           glTexParameterfv ,(GLenum target, GLenum pname, const GLfloat* params));
FNDEF( void,           glTexParameteri, (GLenum target, GLenum pname, GLint param));
FNDEF( void,           glTexParameteriv ,(GLenum target, GLenum pname, const GLint* params));
FNDEF( void,           glTexSubImage2D ,(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels));
FNDEF( void,           glUniform1f, (GLint location, GLfloat x));
FNDEF( void,           glUniform1fv, (GLint location, GLsizei count, const GLfloat* v));
FNDEF( void,           glUniform1i, (GLint location, GLint x));
FNDEF( void,           glUniform1iv, (GLint location, GLsizei count, const GLint* v));
FNDEF( void,           glUniform2f ,(GLint location, GLfloat x, GLfloat y));
FNDEF( void,           glUniform2fv, (GLint location, GLsizei count, const GLfloat* v));
FNDEF( void,           glUniform2i, (GLint location, GLint x, GLint y));
FNDEF( void,           glUniform2iv, (GLint location, GLsizei count, const GLint* v));
FNDEF( void,           glUniform3f ,(GLint location, GLfloat x, GLfloat y, GLfloat z));
FNDEF( void,           glUniform3fv, (GLint location, GLsizei count, const GLfloat* v));
FNDEF( void,           glUniform3i ,(GLint location, GLint x, GLint y, GLint z));
FNDEF( void,           glUniform3iv, (GLint location, GLsizei count, const GLint* v));
FNDEF( void,           glUniform4f ,(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w));
FNDEF( void,           glUniform4fv ,(GLint location, GLsizei count, const GLfloat* v));
FNDEF( void,           glUniform4i, (GLint location, GLint x, GLint y, GLint z, GLint w));
FNDEF( void,           glUniform4iv ,(GLint location, GLsizei count, const GLint* v));
FNDEF( void,           glUniformMatrix2fv ,(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value));
FNDEF( void,           glUniformMatrix3fv ,(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value));
FNDEF( void,           glUniformMatrix4fv ,(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value));
FNDEF( void,           glUseProgram ,(GLuint program));
FNDEF( void,           glValidateProgram, (GLuint program));
FNDEF( void,           glVertexAttrib1f ,(GLuint indx, GLfloat x));
FNDEF( void,           glVertexAttrib1fv, (GLuint indx, const GLfloat* values));
FNDEF( void,           glVertexAttrib2f, (GLuint indx, GLfloat x, GLfloat y));
FNDEF( void,           glVertexAttrib2fv, (GLuint indx, const GLfloat* values));
FNDEF( void,           glVertexAttrib3f, (GLuint indx, GLfloat x, GLfloat y, GLfloat z));
FNDEF( void,           glVertexAttrib3fv, (GLuint indx, const GLfloat* values));
FNDEF( void,           glVertexAttrib4f, (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w));
FNDEF( void,           glVertexAttrib4fv, (GLuint indx, const GLfloat* values));
FNDEF( void,           glVertexAttribPointer, (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr));
FNDEF( void,           glViewport, (GLint x, GLint y, GLsizei width, GLsizei height));
//ghd}


#undef FN
#define FNPTR(name) funcPtr_##name

#ifndef IMPORTGL_NO_FNPTR_DEFS

// Redirect egl* and gl* function calls to funcPtr_egl* and funcPtr_gl*.

#ifndef ANDROID_NDK
#define eglChooseConfig         FNPTR(eglChooseConfig)
#define eglCreateContext        FNPTR(eglCreateContext)
#define eglCreateWindowSurface  FNPTR(eglCreateWindowSurface)
#define eglDestroyContext       FNPTR(eglDestroyContext)
#define eglDestroySurface       FNPTR(eglDestroySurface)
#define eglGetConfigAttrib      FNPTR(eglGetConfigAttrib)
#define eglGetConfigs           FNPTR(eglGetConfigs)
#define eglGetDisplay           FNPTR(eglGetDisplay)
#define eglGetError             FNPTR(eglGetError)
#define eglInitialize           FNPTR(eglInitialize)
#define eglMakeCurrent          FNPTR(eglMakeCurrent)
#define eglSwapBuffers          FNPTR(eglSwapBuffers)
#define eglTerminate            FNPTR(eglTerminate)
#endif /* !ANDROID_NDK */

#define glBlendFunc             FNPTR(glBlendFunc)
#define glClear                 FNPTR(glClear)
#define glClearColorx           FNPTR(glClearColorx)
#define glColor4x               FNPTR(glColor4x)
#define glColorPointer          FNPTR(glColorPointer)
#define glDisable               FNPTR(glDisable)
#define glDisableClientState    FNPTR(glDisableClientState)
#define glDrawArrays            FNPTR(glDrawArrays)
#define glEnable                FNPTR(glEnable)
#define glEnableClientState     FNPTR(glEnableClientState)
#define glFrustumx              FNPTR(glFrustumx)
#define glGetError              FNPTR(glGetError)
#define glLightxv               FNPTR(glLightxv)
#define glLoadIdentity          FNPTR(glLoadIdentity)
#define glMaterialx             FNPTR(glMaterialx)
#define glMaterialxv            FNPTR(glMaterialxv)
#define glMatrixMode            FNPTR(glMatrixMode)
#define glMultMatrixx           FNPTR(glMultMatrixx)
#define glNormalPointer         FNPTR(glNormalPointer)
#define glPopMatrix             FNPTR(glPopMatrix)
#define glPushMatrix            FNPTR(glPushMatrix)
#define glRotatex               FNPTR(glRotatex)
#define glScalex                FNPTR(glScalex)
#define glShadeModel            FNPTR(glShadeModel)
#define glTranslatex            FNPTR(glTranslatex)
#define glVertexPointer         FNPTR(glVertexPointer)
#define glViewport              FNPTR(glViewport)

//ghd {
#define  FNPTR(glActiveTexture)
#define           FNPTR(glAttachShader)
#define           FNPTR(glBindAttribLocation)
#define           FNPTR(glBindBuffer)
#define           FNPTR(glBindFramebuffer)
#define           FNPTR(glBindRenderbuffer)
#define           FNPTR(glBindTexture)
#define           FNPTR(glBlendColor)
#define           FNPTR(glBlendEquation)
#define           FNPTR(glBlendEquationSeparate)
#define           FNPTR(glBlendFuncSeparate)
#define           FNPTR(glBufferData)
#define           FNPTR(glBufferSubData)
#define           FNPTR(glCheckFramebufferStatus)
#define           FNPTR(glClear)
#define           FNPTR(glClearColor)
#define           FNPTR(glClearDepthf)
#define           FNPTR(glClearStencil)
#define           FNPTR(glColorMask)
#define           FNPTR(glCompileShader)
#define           FNPTR(glCompressedTexImage2D)
#define           FNPTR(glCompressedTexSubImage2D)
#define          FNPTR(glCopyTexImage2D)
#define          FNPTR(glCopyTexSubImage2D)
#define          FNPTR(glCreateProgram)
#define          FNPTR(glCreateShader)
#define          FNPTR(glCullFace)
#define          FNPTR(glDeleteBuffers)
#define          FNPTR(glDeleteFramebuffers)
#define          FNPTR(glDeleteProgram)
#define          FNPTR(glDeleteRenderbuffers)
#define          FNPTR(glDeleteShader)
#define          FNPTR(glDeleteTextures)
#define          FNPTR(glDepthFunc)
#define          FNPTR(glDepthMask)
#define          FNPTR(glDepthRangef)
#define          FNPTR(glDetachShader)
#define          FNPTR(glDisable)
#define          FNPTR(glDisableVertexAttribArray)
#define          FNPTR(glDrawArrays)
#define          FNPTR(glDrawElements)
#define          FNPTR(glEnable)
#define          FNPTR(glEnableVertexAttribArray)
#define          FNPTR(glFinish)
#define          FNPTR(glFlush)
#define          FNPTR(glFramebufferRenderbuffer)
#define          FNPTR(glFramebufferTexture2D)
#define          FNPTR(glFrontFace)
#define          FNPTR(glGenBuffers)
#define          FNPTR(glGenerateMipmap)
#define          FNPTR(glGenFramebuffers)
#define          FNPTR(glGenRenderbuffers)
#define          FNPTR(glGenTextures)
#define          FNPTR(glGetActiveAttrib)
#define          FNPTR(glGetActiveUniform)
#define          FNPTR(glGetAttachedShaders)
#define          FNPTR(glGetAttribLocation)
#define          FNPTR(glGetBooleanv)
#define          FNPTR(glGetBufferParameteriv)
#define          FNPTR(glGetError)
#define          FNPTR(glGetFloatv)
#define          FNPTR(glGetFramebufferAttachmentParameteriv)
#define          FNPTR(glGetIntegerv)
#define          FNPTR(glGetProgramiv)
#define          FNPTR(glGetProgramInfoLog)
#define          FNPTR(glGetRenderbufferParameteriv)
#define          FNPTR(glGetShaderiv)
#define          FNPTR(glGetShaderInfoLog)
#define          FNPTR(glGetShaderPrecisionFormat)
#define          FNPTR(glGetShaderSource)
#define    		  FNPTR(glGetString)
#define           FNPTR(glGetTexParameterfv)
#define           FNPTR(glGetTexParameteriv)
#define           FNPTR(glGetUniformfv)
#define           FNPTR(glGetUniformiv)
#define           FNPTR(glGetUniformLocation)
#define           FNPTR(glGetVertexAttribfv)
#define           FNPTR(glGetVertexAttribiv)
#define           FNPTR(glGetVertexAttribPointerv)
#define           FNPTR(glHint)
#define      	  FNPTR(glIsBuffer)
#define       	  FNPTR(glIsEnabled)
#define      	  FNPTR(glIsFramebuffer)
#define       	  FNPTR(glIsProgram)
#define      	  FNPTR(glIsRenderbuffer)
#define       	  FNPTR(glIsShader)
#define       	  FNPTR(glIsTexture)
#define           FNPTR(glLineWidth)
#define           FNPTR(glLinkProgram)
#define           FNPTR(glPixelStorei)
#define           FNPTR(glPolygonOffset)
#define           FNPTR(glReadPixels)
#define           FNPTR(glReleaseShaderCompiler)
#define           FNPTR(glRenderbufferStorage)
#define           FNPTR(glSampleCoverage)
#define           FNPTR(glScissor)
#define           FNPTR(glShaderBinary)
#define           FNPTR(glShaderSource)
#define           FNPTR(glStencilFunc)
#define           FNPTR(glStencilFuncSeparate)
#define           FNPTR(glStencilMask)
#define           FNPTR(glStencilMaskSeparate)
#define           FNPTR(glStencilOp)
#define           FNPTR(glStencilOpSeparate)
#define           FNPTR(glTexImage2D)
#define           FNPTR(glTexParameterf)
#define           FNPTR(glTexParameterfv)
#define           FNPTR(glTexParameteri)
#define           FNPTR(glTexParameteriv)
#define           FNPTR(glTexSubImage2D)
#define           FNPTR(glUniform1f)
#define           FNPTR(glUniform1fv)
#define           FNPTR(glUniform1i)
#define           FNPTR(glUniform1iv)
#define           FNPTR(glUniform2f)
#define           FNPTR(glUniform2fv)
#define           FNPTR(glUniform2i)
#define           FNPTR(glUniform2iv)
#define           FNPTR(glUniform3f)
#define           FNPTR(glUniform3fv)
#define           FNPTR(glUniform3i)
#define           FNPTR(glUniform3iv)
#define           FNPTR(glUniform4f)
#define           FNPTR(glUniform4fv)
#define           FNPTR(glUniform4i)
#define           FNPTR(glUniform4iv)
#define           FNPTR(glUniformMatrix2fv)
#define           FNPTR(glUniformMatrix3fv)
#define           FNPTR(glUniformMatrix4fv)
#define           FNPTR(glUseProgram)
#define           FNPTR(glValidateProgram)
#define           FNPTR(glVertexAttrib1f)
#define           FNPTR(glVertexAttrib1fv)
#define           FNPTR(glVertexAttrib2f)
#define           FNPTR(glVertexAttrib2fv)
#define           FNPTR(glVertexAttrib3f)
#define           FNPTR(glVertexAttrib3fv)
#define           FNPTR(glVertexAttrib4f)
#define           FNPTR(glVertexAttrib4fv)
#define           FNPTR(glVertexAttribPointer)
#define           FNPTR(glViewport)

//ghd }

#endif // !IMPORTGL_NO_FNPTR_DEFS


#endif // !DISABLE_IMPORTGL


#ifdef __cplusplus
}
#endif


#endif // !IMPORTGL_H_INCLUDED
