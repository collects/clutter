/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Authored By Matthew Allum  <mallum@openedhand.com>
 *
 * Copyright (C) 2006 OpenedHand
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

/**
 * SECTION:clutter-egl
 * @short_description: EGL specific API
 *
 * The EGL backend for Clutter provides some EGL specific API
 */

#ifndef __CLUTTER_EGL_H__
#define __CLUTTER_EGL_H__

#include <glib.h>

#ifdef COGL_HAS_XLIB_SUPPORT
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#endif

#include "clutter-egl-headers.h"

#include <clutter/clutter-stage.h>

G_BEGIN_DECLS

#ifdef COGL_HAS_XLIB_SUPPORT
/**
 * clutter_eglx_display:
 *
 * Retrieves the <structname>EGLDisplay</structname> used by Clutter
 *
 * Return value: the EGL display
 *
 * Since: 0.4
 */
EGLDisplay
clutter_eglx_display (void);
#endif

/**
 * clutter_egl_display:
 *
 * Retrieves the <structname>EGLDisplay</structname> used by Clutter
 *
 * Return value: the EGL display
 */
EGLDisplay
clutter_egl_display (void);

G_END_DECLS

#endif /* __CLUTTER_EGL_H__ */