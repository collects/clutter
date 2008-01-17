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
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:clutter-behaviour-scale
 * @short_description: A behaviour controlling scale
 *
 * A #ClutterBehaviourScale interpolates actors size between two values.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "clutter-actor.h"
#include "clutter-behaviour.h"
#include "clutter-enum-types.h"
#include "clutter-main.h"
#include "clutter-fixed.h"
#include "clutter-behaviour-scale.h"
#include "clutter-private.h"
#include "clutter-debug.h"

#include <math.h>

G_DEFINE_TYPE (ClutterBehaviourScale,
               clutter_behaviour_scale,
	       CLUTTER_TYPE_BEHAVIOUR);

struct _ClutterBehaviourScalePrivate
{
  ClutterFixed scale_start[2];
  ClutterFixed scale_end[2];

  ClutterGravity gravity;
};

#define CLUTTER_BEHAVIOUR_SCALE_GET_PRIVATE(obj)        \
              (G_TYPE_INSTANCE_GET_PRIVATE ((obj),      \
               CLUTTER_TYPE_BEHAVIOUR_SCALE,            \
               ClutterBehaviourScalePrivate))

enum
{
  PROP_0,

  PROP_X_SCALE_START,
  PROP_Y_SCALE_START,
  PROP_X_SCALE_END,
  PROP_Y_SCALE_END,
  PROP_SCALE_GRAVITY
};

typedef struct {
  ClutterFixed scale_x;
  ClutterFixed scale_y;
} ScaleFrameClosure;

static void
scale_frame_foreach (ClutterBehaviour *behaviour,
                     ClutterActor     *actor,
		     gpointer          data)
{
  ClutterBehaviourScalePrivate *priv =
      CLUTTER_BEHAVIOUR_SCALE (behaviour)->priv;
  ScaleFrameClosure *closure = data;
  ClutterGravity gravity = priv->gravity;

  /* Don't mess with the actor anchor point of gravity is set to
   * none
   */
  if (gravity != CLUTTER_GRAVITY_NONE)
    clutter_actor_set_anchor_point_from_gravity (actor, gravity);

  clutter_actor_set_scalex (actor, closure->scale_x, closure->scale_y);
}

static void
clutter_behaviour_scale_alpha_notify (ClutterBehaviour *behave,
                                      guint32           alpha_value)
{
  ClutterBehaviourScalePrivate *priv;
  ClutterFixed scale_x, scale_y, factor;
  ScaleFrameClosure closure = { 0, };

  priv = CLUTTER_BEHAVIOUR_SCALE (behave)->priv;

  factor = CLUTTER_INT_TO_FIXED (alpha_value) / CLUTTER_ALPHA_MAX_ALPHA;

  scale_x = CLUTTER_FIXED_MUL (factor,
                               (priv->scale_end[0] - priv->scale_start[0]));
  scale_x += priv->scale_start[0];

  scale_y = CLUTTER_FIXED_MUL (factor,
                               (priv->scale_end[1] - priv->scale_start[1]));
  scale_y += priv->scale_start[1];

  closure.scale_x = scale_x;
  closure.scale_y = scale_y;

  clutter_behaviour_actors_foreach (behave,
                                    scale_frame_foreach,
                                    &closure);
}

static void
clutter_behaviour_scale_set_property (GObject      *gobject,
                                      guint         prop_id,
                                      const GValue *value,
                                      GParamSpec   *pspec)
{
  ClutterBehaviourScalePrivate *priv;

  priv = CLUTTER_BEHAVIOUR_SCALE (gobject)->priv;

  switch (prop_id)
    {
    case PROP_X_SCALE_START:
      priv->scale_start[0] =
        CLUTTER_FLOAT_TO_FIXED (g_value_get_double (value));
      break;
    case PROP_X_SCALE_END:
      priv->scale_end[0] =
        CLUTTER_FLOAT_TO_FIXED (g_value_get_double (value));
      break;
    case PROP_Y_SCALE_START:
      priv->scale_start[1] =
        CLUTTER_FLOAT_TO_FIXED (g_value_get_double (value));
      break;
    case PROP_Y_SCALE_END:
      priv->scale_end[1] =
        CLUTTER_FLOAT_TO_FIXED (g_value_get_double (value));
      break;
    case PROP_SCALE_GRAVITY:
      priv->gravity = g_value_get_enum (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
      break;
    }
}

static void
clutter_behaviour_scale_get_property (GObject    *gobject,
                                      guint       prop_id,
                                      GValue     *value,
                                      GParamSpec *pspec)
{
  ClutterBehaviourScalePrivate *priv;

  priv = CLUTTER_BEHAVIOUR_SCALE (gobject)->priv;

  switch (prop_id)
    {
    case PROP_X_SCALE_START:
      g_value_set_double (value, CLUTTER_FIXED_TO_FLOAT (priv->scale_start[0]));
      break;
    case PROP_X_SCALE_END:
      g_value_set_double (value, CLUTTER_FIXED_TO_FLOAT (priv->scale_end[0]));
      break;
    case PROP_Y_SCALE_START:
      g_value_set_double (value, CLUTTER_FIXED_TO_FLOAT (priv->scale_start[1]));
      break;
    case PROP_Y_SCALE_END:
      g_value_set_double (value, CLUTTER_FIXED_TO_FLOAT (priv->scale_end[1]));
      break;
    case PROP_SCALE_GRAVITY:
      g_value_set_enum (value, priv->gravity);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
      break;
    }
}

static void
clutter_behaviour_scale_class_init (ClutterBehaviourScaleClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  ClutterBehaviourClass *behave_class = CLUTTER_BEHAVIOUR_CLASS (klass);

  gobject_class->set_property = clutter_behaviour_scale_set_property;
  gobject_class->get_property = clutter_behaviour_scale_get_property;

  /**
   * ClutterBehaviourScale:x-scale-start:
   *
   * The initial scaling factor on the X axis for the actors.
   *
   * Since: 0.6
   */
  g_object_class_install_property (gobject_class,
                                   PROP_X_SCALE_START,
                                   g_param_spec_double ("x-scale-start",
                                                        "X Start Scale",
                                                        "Initial scale on the X axis",
                                                        0.0, G_MAXDOUBLE,
                                                        1.0,
                                                        CLUTTER_PARAM_READWRITE));
  /**
   * ClutterBehaviourScale:x-scale-end:
   *
   * The final scaling factor on the X axis for the actors.
   *
   * Since: 0.6
   */
  g_object_class_install_property (gobject_class,
                                   PROP_X_SCALE_END,
                                   g_param_spec_double ("x-scale-end",
                                                        "X End Scale",
                                                        "Final scale on the X axis",
                                                        0.0, G_MAXDOUBLE,
                                                        1.0,
                                                        CLUTTER_PARAM_READWRITE));
  /**
   * ClutterBehaviourScale:y-scale-start:
   *
   * The initial scaling factor on the Y axis for the actors.
   *
   * Since: 0.6
   */
  g_object_class_install_property (gobject_class,
                                   PROP_Y_SCALE_START,
                                   g_param_spec_double ("y-scale-start",
                                                        "Y Start Scale",
                                                        "Initial scale on the Y axis",
                                                        0.0, G_MAXDOUBLE,
                                                        1.0,
                                                        CLUTTER_PARAM_READWRITE));
  /**
   * ClutterBehaviourScale:y-scale-end:
   *
   * The final scaling factor on the Y axis for the actors.
   *
   * Since: 0.6
   */
  g_object_class_install_property (gobject_class,
                                   PROP_X_SCALE_END,
                                   g_param_spec_double ("y-scale-end",
                                                        "Y End Scale",
                                                        "Final scale on the Y axis",
                                                        0.0, G_MAXDOUBLE,
                                                        1.0,
                                                        CLUTTER_PARAM_READWRITE));
  /**
   * ClutterBehaviourScale:gravity:
   *
   * The gravity of the scaling.
   *
   * Since: 0.2
   */
  g_object_class_install_property (gobject_class,
                                   PROP_SCALE_GRAVITY,
                                   g_param_spec_enum ("scale-gravity",
                                                      "Scale Gravity",
                                                      "The gravity of the scaling",
                                                      CLUTTER_TYPE_GRAVITY,
                                                      CLUTTER_GRAVITY_CENTER,
                                                      CLUTTER_PARAM_READWRITE));


  behave_class->alpha_notify = clutter_behaviour_scale_alpha_notify;

  g_type_class_add_private (klass, sizeof (ClutterBehaviourScalePrivate));
}

static void
clutter_behaviour_scale_init (ClutterBehaviourScale *self)
{
  ClutterBehaviourScalePrivate *priv;

  self->priv = priv = CLUTTER_BEHAVIOUR_SCALE_GET_PRIVATE (self);
}

/**
 * clutter_behaviour_scale_new:
 * @alpha: a #ClutterAlpha
 * @x_scale_start: initial scale factor on the X axis
 * @y_scale_start: initial scale factor on the Y axis
 * @x_scale_end: final scale factor on the X axis
 * @y_scale_end: final scale factor on the Y axis
 * @gravity: a #ClutterGravity for the scale.
 *
 * Creates a new  #ClutterBehaviourScale instance.
 *
 * Return value: the newly created #ClutterBehaviourScale
 *
 * Since: 0.2
 */
ClutterBehaviour *
clutter_behaviour_scale_new (ClutterAlpha   *alpha,
			     gdouble         x_scale_start,
			     gdouble         y_scale_start,
			     gdouble         x_scale_end,
			     gdouble         y_scale_end,
			     ClutterGravity  gravity)
{
  g_return_val_if_fail (alpha == NULL || CLUTTER_IS_ALPHA (alpha), NULL);

  return clutter_behaviour_scale_newx (alpha,
				       CLUTTER_FLOAT_TO_FIXED (x_scale_start),
				       CLUTTER_FLOAT_TO_FIXED (y_scale_start),
				       CLUTTER_FLOAT_TO_FIXED (x_scale_end),
				       CLUTTER_FLOAT_TO_FIXED (y_scale_end),
				       gravity);
}

/**
 * clutter_behaviour_scale_newx:
 * @alpha: a #ClutterAlpha
 * @x_scale_start: initial scale factor on the X axis
 * @y_scale_start: initial scale factor on the Y axis
 * @x_scale_end: final scale factor on the X axis
 * @y_scale_end: final scale factor on the Y axis
 * @gravity: a #ClutterGravity for the scale.
 *
 * A fixed point implementation of clutter_behaviour_scale_new()
 *
 * Return value: the newly created #ClutterBehaviourScale
 *
 * Since: 0.2
 */
ClutterBehaviour *
clutter_behaviour_scale_newx (ClutterAlpha   *alpha,
			      ClutterFixed    x_scale_start,
			      ClutterFixed    y_scale_start,
			      ClutterFixed    x_scale_end,
			      ClutterFixed    y_scale_end,
			      ClutterGravity  gravity)
{
  ClutterBehaviourScale *behave;

  g_return_val_if_fail (alpha == NULL || CLUTTER_IS_ALPHA (alpha), NULL);

  behave = g_object_new (CLUTTER_TYPE_BEHAVIOUR_SCALE, "alpha", alpha, NULL);

  behave->priv->scale_start[0] = x_scale_start;
  behave->priv->scale_start[1] = y_scale_start;
  behave->priv->scale_end[0]   = x_scale_end;
  behave->priv->scale_end[1]   = y_scale_end;

  behave->priv->gravity = gravity;

  return CLUTTER_BEHAVIOUR (behave);
}

/**
 * clutter_behaviour_scale_set_bounds:
 * @scale: a #ClutterBehaviourScale
 * @x_scale_start: initial scale factor on the X axis
 * @y_scale_start: initial scale factor on the Y axis
 * @x_scale_end: final scale factor on the X axis
 * @y_scale_end: final scale factor on the Y axis
 *
 * Sets the bounds used by scale behaviour.
 *
 * Since: 0.6
 */
void
clutter_behaviour_scale_set_bounds (ClutterBehaviourScale *scale,
                                    gdouble                x_scale_start,
                                    gdouble                y_scale_start,
                                    gdouble                x_scale_end,
                                    gdouble                y_scale_end)
{
  g_return_if_fail (CLUTTER_IS_BEHAVIOUR_SCALE (scale));

  clutter_behaviour_scale_set_boundsx (scale,
                                       CLUTTER_FLOAT_TO_FIXED (x_scale_start),
                                       CLUTTER_FLOAT_TO_FIXED (y_scale_start),
                                       CLUTTER_FLOAT_TO_FIXED (x_scale_end),
                                       CLUTTER_FLOAT_TO_FIXED (y_scale_end));
}

/**
 * clutter_behaviour_scale_get_bounds:
 * @scale: a #ClutterBehaviourScale
 * @x_scale_start: return location for the initial scale factor on the X
 *   axis, or %NULL
 * @y_scale_start: return location for the initial scale factor on the Y
 *   axis, or %NULL
 * @x_scale_end: return location for the final scale factor on the X axis,
 *   or %NULL
 * @y_scale_end: return location for the final scale factor on the Y axis,
 *   or %NULL
 *
 * Retrieves the bounds used by scale behaviour.
 *
 * Since: 0.4
 */
void
clutter_behaviour_scale_get_bounds (ClutterBehaviourScale *scale,
                                    gdouble               *x_scale_start,
                                    gdouble               *y_scale_start,
                                    gdouble               *x_scale_end,
                                    gdouble               *y_scale_end)
{
  ClutterBehaviourScalePrivate *priv;

  g_return_if_fail (CLUTTER_IS_BEHAVIOUR_SCALE (scale));

  priv = scale->priv;

  if (x_scale_start)
    *x_scale_start = CLUTTER_FIXED_TO_DOUBLE (priv->scale_start[0]);

  if (x_scale_end)
    *x_scale_end = CLUTTER_FIXED_TO_DOUBLE (priv->scale_end[0]);

  if (y_scale_start)
    *y_scale_start = CLUTTER_FIXED_TO_DOUBLE (priv->scale_start[1]);

  if (y_scale_end)
    *y_scale_end = CLUTTER_FIXED_TO_DOUBLE (priv->scale_end[1]);
}

/**
 * clutter_behaviour_scale_set_boundsx:
 * @scale: a #ClutterBehaviourScale
 * @x_scale_start: initial scale factor on the X axis
 * @y_scale_start: initial scale factor on the Y axis
 * @x_scale_end: final scale factor on the X axis
 * @y_scale_end: final scale factor on the Y axis
 *
 * Fixed point version of clutter_behaviour_scale_set_bounds().
 *
 * Sets the bounds used by scale behaviour.
 *
 * Since: 0.6
 */
void
clutter_behaviour_scale_set_boundsx (ClutterBehaviourScale *scale,
                                     ClutterFixed           x_scale_start,
                                     ClutterFixed           y_scale_start,
                                     ClutterFixed           x_scale_end,
                                     ClutterFixed           y_scale_end)
{
  ClutterBehaviourScalePrivate *priv;

  g_return_if_fail (CLUTTER_IS_BEHAVIOUR_SCALE (scale));

  priv = scale->priv;

  g_object_freeze_notify (G_OBJECT (scale));

  if (priv->scale_start[0] != x_scale_start)
    {
      priv->scale_start[0] = x_scale_start;
      g_object_notify (G_OBJECT (scale), "x-scale-start");
    }

  if (priv->scale_start[1] != y_scale_start)
    {
      priv->scale_start[1] = y_scale_start;
      g_object_notify (G_OBJECT (scale), "y-scale-start");
    }

  if (priv->scale_end[0] != x_scale_end)
    {
      priv->scale_end[0]   = x_scale_end;
      g_object_notify (G_OBJECT (scale), "x-scale-end");
    }

  if (priv->scale_end[1] != y_scale_end)
    {
      priv->scale_end[1]   = y_scale_end;
      g_object_notify (G_OBJECT (scale), "y-scale-end");
    }

  g_object_thaw_notify (G_OBJECT (scale));
}

/**
 * clutter_behaviour_scale_get_boundsx:
 * @scale: a #ClutterBehaviourScale
 * @x_scale_start: return location for the initial scale factor on the X
 *   axis, or %NULL
 * @y_scale_start: return location for the initial scale factor on the Y
 *   axis, or %NULL
 * @x_scale_end: return location for the final scale factor on the X axis,
 *   or %NULL
 * @y_scale_end: return location for the final scale factor on the Y axis,
 *   or %NULL
 *
 * Fixed point version of clutter_behaviour_scale_get_bounds().
 *
 * Retrieves the bounds used by scale behaviour.
 *
 * Since: 0.4
 */
void
clutter_behaviour_scale_get_boundsx (ClutterBehaviourScale *scale,
                                     ClutterFixed          *x_scale_start,
                                     ClutterFixed          *y_scale_start,
                                     ClutterFixed          *x_scale_end,
                                     ClutterFixed          *y_scale_end)
{
  ClutterBehaviourScalePrivate *priv;

  g_return_if_fail (CLUTTER_IS_BEHAVIOUR_SCALE (scale));

  priv = scale->priv;

  if (x_scale_start)
    *x_scale_start = priv->scale_start[0];

  if (x_scale_end)
    *x_scale_end = priv->scale_end[0];

  if (y_scale_start)
    *y_scale_start = priv->scale_start[1];

  if (y_scale_end)
    *y_scale_end = priv->scale_end[1];
}

/**
 * clutter_behaviour_scale_set_gravity:
 * @scale: a #ClutterBehaviourScale
 * @gravity: the gravity of the scaling
 *
 * Sets the #ClutterGravity applied by the scale behaviour.
 *
 * Since: 0.6
 */
void
clutter_behaviour_scale_set_gravity (ClutterBehaviourScale *scale,
                                     ClutterGravity         gravity)
{
  g_return_if_fail (CLUTTER_IS_BEHAVIOUR_SCALE (scale));

  if (scale->priv->gravity != gravity)
    {
      scale->priv->gravity = gravity;

      g_object_notify (G_OBJECT (scale), "scale-gravity");
    }
}
/**
 * clutter_behaviour_scale_get_gravity:
 * @scale: a #ClutterBehaviourScale
 *
 * Retrieves the #ClutterGravity applied by the scale behaviour.
 *
 * Return value: the gravity used by the behaviour
 *
 * Since: 0.4
 */
ClutterGravity
clutter_behaviour_scale_get_gravity (ClutterBehaviourScale *scale)
{
  g_return_val_if_fail (CLUTTER_IS_BEHAVIOUR_SCALE (scale), CLUTTER_GRAVITY_NONE);

  return scale->priv->gravity;
}
