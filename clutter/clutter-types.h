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
 */

#if !defined(__CLUTTER_H_INSIDE__) && !defined(CLUTTER_COMPILATION)
#error "Only <clutter/clutter.h> can be included directly."
#endif

#ifndef __CLUTTER_TYPES_H__
#define __CLUTTER_TYPES_H__

#include <clutter/clutter-enums.h>

G_BEGIN_DECLS

/* these macros are used to mark deprecated functions, and thus have to be
 * exposed in a public header.
 *
 * do *not* use them in other libraries depending on Clutter: use G_DEPRECATED
 * and G_DEPRECATED_FOR, or use your own wrappers around them.
 */
#ifdef CLUTTER_DISABLE_DEPRECATION_WARNINGS
#define CLUTTER_DEPRECATED
#define CLUTTER_DEPRECATED_FOR(f)
#else
#define CLUTTER_DEPRECATED G_DEPRECATED
#define CLUTTER_DEPRECATED_FOR(f) G_DEPRECATED_FOR(f)
#endif

#define CLUTTER_TYPE_ACTOR_BOX          (clutter_actor_box_get_type ())
#define CLUTTER_TYPE_GEOMETRY           (clutter_geometry_get_type ())
#define CLUTTER_TYPE_KNOT               (clutter_knot_get_type ())
#define CLUTTER_TYPE_PAINT_VOLUME       (clutter_paint_volume_get_type ())
#define CLUTTER_TYPE_VERTEX             (clutter_vertex_get_type ())

typedef struct _ClutterActor            ClutterActor;

typedef struct _ClutterStage            ClutterStage;
typedef struct _ClutterContainer        ClutterContainer; /* dummy */
typedef struct _ClutterChildMeta        ClutterChildMeta;
typedef struct _ClutterLayoutMeta       ClutterLayoutMeta;
typedef struct _ClutterActorMeta        ClutterActorMeta;

typedef struct _ClutterAlpha            ClutterAlpha;
typedef struct _ClutterAnimatable       ClutterAnimatable; /* dummy */
typedef struct _ClutterAnimator         ClutterAnimator;
typedef struct _ClutterState            ClutterState;
typedef struct _ClutterTimeline         ClutterTimeline;

typedef struct _ClutterAction           ClutterAction;
typedef struct _ClutterConstraint       ClutterConstraint;
typedef struct _ClutterEffect           ClutterEffect;

typedef struct _ClutterPath             ClutterPath;

#if !defined(CLUTTER_DISABLE_DEPRECATED) || defined(CLUTTER_COMPILATION)
typedef struct _ClutterBehaviour        ClutterBehaviour;

typedef struct _ClutterShader           ClutterShader;
#endif /* DISABLE_DEPRECATED */

typedef struct _ClutterActorBox         ClutterActorBox;
typedef struct _ClutterColor            ClutterColor;
typedef struct _ClutterGeometry         ClutterGeometry;
typedef struct _ClutterKnot             ClutterKnot;
typedef struct _ClutterVertex           ClutterVertex;

typedef union _ClutterEvent             ClutterEvent;

/**
 * ClutterPaintVolume:
 *
 * <structname>ClutterPaintVolume</structname> is an opaque structure
 * whose members cannot be directly accessed.
 *
 * A <structname>ClutterPaintVolume</structname> represents an
 * a bounding volume whos internal representation isn't defined but
 * can be set and queried in terms of an axis aligned bounding box.
 *
 * Other internal representation and methods for describing the
 * bounding volume may be added in the future.
 *
 * Since: 1.4
 */
typedef struct _ClutterPaintVolume      ClutterPaintVolume;

/**
 * ClutterVertex:
 * @x: X coordinate of the vertex
 * @y: Y coordinate of the vertex
 * @z: Z coordinate of the vertex
 *
 * Vertex of an actor in 3D space, expressed in pixels
 *
 * Since: 0.4
 */
struct _ClutterVertex
{
  gfloat x;
  gfloat y;
  gfloat z;
};

GType          clutter_vertex_get_type (void) G_GNUC_CONST;
ClutterVertex *clutter_vertex_new      (gfloat               x,
                                        gfloat               y,
                                        gfloat               z);
ClutterVertex *clutter_vertex_copy     (const ClutterVertex *vertex);
void           clutter_vertex_free     (ClutterVertex       *vertex);
gboolean       clutter_vertex_equal    (const ClutterVertex *vertex_a,
                                        const ClutterVertex *vertex_b);

/**
 * ClutterActorBox:
 * @x1: X coordinate of the top left corner
 * @y1: Y coordinate of the top left corner
 * @x2: X coordinate of the bottom right corner
 * @y2: Y coordinate of the bottom right corner
 *
 * Bounding box of an actor. The coordinates of the top left and right bottom
 * corners of an actor. The coordinates of the two points are expressed in
 * pixels with sub-pixel precision
 */
struct _ClutterActorBox
{
  gfloat x1;
  gfloat y1;

  gfloat x2;
  gfloat y2;
};

GType            clutter_actor_box_get_type      (void) G_GNUC_CONST;
ClutterActorBox *clutter_actor_box_new           (gfloat                 x_1,
                                                  gfloat                 y_1,
                                                  gfloat                 x_2,
                                                  gfloat                 y_2);
ClutterActorBox *clutter_actor_box_copy          (const ClutterActorBox *box);
void             clutter_actor_box_free          (ClutterActorBox       *box);
gboolean         clutter_actor_box_equal         (const ClutterActorBox *box_a,
                                                  const ClutterActorBox *box_b);
gfloat           clutter_actor_box_get_x         (const ClutterActorBox *box);
gfloat           clutter_actor_box_get_y         (const ClutterActorBox *box);
gfloat           clutter_actor_box_get_width     (const ClutterActorBox *box);
gfloat           clutter_actor_box_get_height    (const ClutterActorBox *box);
void             clutter_actor_box_get_origin    (const ClutterActorBox *box,
                                                  gfloat                *x,
                                                  gfloat                *y);
void             clutter_actor_box_get_size      (const ClutterActorBox *box,
                                                  gfloat                *width,
                                                  gfloat                *height);
gfloat           clutter_actor_box_get_area      (const ClutterActorBox *box);
gboolean         clutter_actor_box_contains      (const ClutterActorBox *box,
                                                  gfloat                 x,
                                                  gfloat                 y);
void             clutter_actor_box_from_vertices (ClutterActorBox       *box,
                                                  const ClutterVertex    verts[]);
void             clutter_actor_box_interpolate   (const ClutterActorBox *initial,
                                                  const ClutterActorBox *final,
                                                  gdouble                progress,
                                                  ClutterActorBox       *result);
void             clutter_actor_box_clamp_to_pixel (ClutterActorBox       *box);
void             clutter_actor_box_union          (const ClutterActorBox *a,
                                                   const ClutterActorBox *b,
                                                   ClutterActorBox       *result);

void             clutter_actor_box_set_origin     (ClutterActorBox       *box,
                                                   gfloat                 x,
                                                   gfloat                 y);
void             clutter_actor_box_set_size       (ClutterActorBox       *box,
                                                   gfloat                 width,
                                                   gfloat                 height);

/**
 * ClutterGeometry:
 * @x: X coordinate of the top left corner of an actor
 * @y: Y coordinate of the top left corner of an actor
 * @width: width of an actor
 * @height: height of an actor
 *
 * The rectangle containing an actor's bounding box, measured in pixels.
 */
struct _ClutterGeometry
{
  /*< public >*/
  gint   x;
  gint   y;
  guint  width;
  guint  height;
};

GType clutter_geometry_get_type (void) G_GNUC_CONST;

void      clutter_geometry_union      (const ClutterGeometry *geometry_a,
                                       const ClutterGeometry *geometry_b,
                                       ClutterGeometry       *result);
gboolean  clutter_geometry_intersects (const ClutterGeometry *geometry0,
                                       const ClutterGeometry *geometry1);

/**
 * ClutterKnot:
 * @x: X coordinate of the knot
 * @y: Y coordinate of the knot
 *
 * Point in a path behaviour.
 *
 * Since: 0.2
 */
struct _ClutterKnot
{
  gint x;
  gint y;
};

GType        clutter_knot_get_type (void) G_GNUC_CONST;
ClutterKnot *clutter_knot_copy     (const ClutterKnot *knot);
void         clutter_knot_free     (ClutterKnot       *knot);
gboolean     clutter_knot_equal    (const ClutterKnot *knot_a,
                                    const ClutterKnot *knot_b);

GType clutter_paint_volume_get_type (void) G_GNUC_CONST;

ClutterPaintVolume *clutter_paint_volume_copy                (const ClutterPaintVolume *pv);
void                clutter_paint_volume_free                (ClutterPaintVolume       *pv);

void                clutter_paint_volume_set_origin          (ClutterPaintVolume       *pv,
                                                              const ClutterVertex      *origin);
void                clutter_paint_volume_get_origin          (const ClutterPaintVolume *pv,
                                                              ClutterVertex            *vertex);
void                clutter_paint_volume_set_width           (ClutterPaintVolume       *pv,
                                                              gfloat                    width);
gfloat              clutter_paint_volume_get_width           (const ClutterPaintVolume *pv);
void                clutter_paint_volume_set_height          (ClutterPaintVolume       *pv,
                                                              gfloat                    height);
gfloat              clutter_paint_volume_get_height          (const ClutterPaintVolume *pv);
void                clutter_paint_volume_set_depth           (ClutterPaintVolume       *pv,
                                                              gfloat                    depth);
gfloat              clutter_paint_volume_get_depth           (const ClutterPaintVolume *pv);
void                clutter_paint_volume_union               (ClutterPaintVolume       *pv,
                                                              const ClutterPaintVolume *another_pv);

gboolean            clutter_paint_volume_set_from_allocation (ClutterPaintVolume       *pv,
                                                              ClutterActor             *actor);

G_END_DECLS

#endif /* __CLUTTER_TYPES_H__ */
