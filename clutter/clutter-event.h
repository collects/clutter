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

#ifndef __CLUTTER_EVENT_H__
#define __CLUTTER_EVENT_H__

#include <clutter/clutter-types.h>
#include <clutter/clutter-input-device.h>

#define CLUTTER_TYPE_EVENT	(clutter_event_get_type ())

/**
 * CLUTTER_PRIORITY_EVENTS:
 *
 * Priority for event handling.
 *
 * Since: 0.4
 */
#define CLUTTER_PRIORITY_EVENTS (G_PRIORITY_DEFAULT)

/**
 * CLUTTER_CURRENT_TIME:
 *
 * Default value for "now".
 *
 * Since: 0.4
 */
#define CLUTTER_CURRENT_TIME    0L

G_BEGIN_DECLS

typedef struct _ClutterAnyEvent         ClutterAnyEvent;
typedef struct _ClutterButtonEvent      ClutterButtonEvent;
typedef struct _ClutterKeyEvent         ClutterKeyEvent;
typedef struct _ClutterMotionEvent      ClutterMotionEvent;
typedef struct _ClutterScrollEvent      ClutterScrollEvent;
typedef struct _ClutterStageStateEvent  ClutterStageStateEvent;
typedef struct _ClutterCrossingEvent    ClutterCrossingEvent;

/**
 * ClutterAnyEvent:
 * @type: event type
 * @time: event time
 * @flags: event flags
 * @source: event source actor
 *
 * Common members for a #ClutterEvent
 *
 * Since: 0.2
 */
struct _ClutterAnyEvent
{
  ClutterEventType type;
  guint32 time;
  ClutterEventFlags flags;
  ClutterStage *stage;
  ClutterActor *source;
};

/**
 * ClutterKeyEvent:
 * @type: event type
 * @time: event time
 * @flags: event flags
 * @stage: event source stage
 * @source: event source actor
 * @modifier_state: key modifiers
 * @keyval: raw key value
 * @hardware_keycode: raw hardware key value
 * @unicode_value: Unicode representation
 * @device: reserved for future use
 *
 * Key event
 *
 * Since: 0.2
 */
struct _ClutterKeyEvent
{
  ClutterEventType type;
  guint32 time;
  ClutterEventFlags flags;
  ClutterStage *stage;
  ClutterActor *source;

  ClutterModifierType modifier_state;
  guint keyval;
  guint16 hardware_keycode;
  gunichar unicode_value;
  ClutterInputDevice *device;
};

/**
 * ClutterButtonEvent:
 * @type: event type
 * @time: event time
 * @flags: event flags
 * @stage: event source stage
 * @source: event source actor
 * @x: event X coordinate, relative to the stage
 * @y: event Y coordinate, relative to the stage
 * @modifier_state: button modifiers
 * @button: event button
 * @click_count: number of button presses within the default time
 *   and radius
 * @axes: reserved for future use
 * @device: reserved for future use
 *
 * Button event.
 *
 * The event coordinates are relative to the stage that received the
 * event, and can be transformed into actor-relative coordinates by
 * using clutter_actor_transform_stage_point().
 *
 * Since: 0.2
 */
struct _ClutterButtonEvent
{
  ClutterEventType type;
  guint32 time;
  ClutterEventFlags flags;
  ClutterStage *stage;
  ClutterActor *source;

  gfloat x;
  gfloat y;
  ClutterModifierType modifier_state;
  guint32 button;
  guint click_count;
  gdouble *axes; /* Future use */
  ClutterInputDevice *device;
};

/**
 * ClutterCrossingEvent:
 * @type: event type
 * @time: event time
 * @flags: event flags
 * @stage: event source stage
 * @source: event source actor
 * @x: event X coordinate
 * @y: event Y coordinate
 * @related: actor related to the crossing
 * @device: reserved for future use
 *
 * Event for the movement of the pointer across different actors
 *
 * Since: 0.2
 */
struct _ClutterCrossingEvent
{
  ClutterEventType type;
  guint32 time;
  ClutterEventFlags flags;
  ClutterStage *stage;
  ClutterActor *source;

  gfloat x;
  gfloat y;
  ClutterInputDevice *device;
  ClutterActor *related;
};

/**
 * ClutterMotionEvent:
 * @type: event type
 * @time: event time
 * @flags: event flags
 * @stage: event source stage
 * @source: event source actor
 * @x: event X coordinate
 * @y: event Y coordinate
 * @modifier_state: button modifiers
 * @axes: reserved for future use
 * @device: reserved for future use
 *
 * Event for the pointer motion
 *
 * Since: 0.2
 */
struct _ClutterMotionEvent
{
  ClutterEventType type;
  guint32 time;
  ClutterEventFlags flags;
  ClutterStage *stage;
  ClutterActor *source;

  gfloat x;
  gfloat y;
  ClutterModifierType modifier_state;
  gdouble *axes; /* Future use */
  ClutterInputDevice *device;
};

/**
 * ClutterScrollEvent:
 * @type: event type
 * @time: event time
 * @flags: event flags
 * @stage: event source stage
 * @source: event source actor
 * @x: event X coordinate
 * @y: event Y coordinate
 * @direction: direction of the scrolling
 * @modifier_state: button modifiers
 * @axes: reserved for future use
 * @device: reserved for future use
 *
 * Scroll wheel (or similar device) event
 *
 * Since: 0.2
 */
struct _ClutterScrollEvent
{
  ClutterEventType type;
  guint32 time;
  ClutterEventFlags flags;
  ClutterStage *stage;
  ClutterActor *source;

  gfloat x;
  gfloat y;
  ClutterScrollDirection direction;
  ClutterModifierType modifier_state;
  gdouble *axes; /* future use */
  ClutterInputDevice *device;
};

/**
 * ClutterStageStateEvent:
 * @type: event type
 * @time: event time
 * @flags: event flags
 * @stage: event source stage
 * @source: event source actor (unused)
 * @changed_mask: bitwise OR of the changed flags
 * @new_state: bitwise OR of the current state flags
 *
 * Event signalling a change in the #ClutterStage state.
 *
 * Since: 0.2
 */
struct _ClutterStageStateEvent
{
  ClutterEventType type;
  guint32 time;
  ClutterEventFlags flags;
  ClutterStage *stage;
  ClutterActor *source; /* XXX: should probably be the stage itself */

  ClutterStageState changed_mask;
  ClutterStageState new_state;
};

/**
 * ClutterEvent:
 *
 * Generic event wrapper.
 *
 * Since: 0.2
 */
union _ClutterEvent
{
  /*< private >*/
  ClutterEventType type;

  ClutterAnyEvent any;
  ClutterButtonEvent button;
  ClutterKeyEvent key;
  ClutterMotionEvent motion;
  ClutterScrollEvent scroll;
  ClutterStageStateEvent stage_state;
  ClutterCrossingEvent crossing;
};

GType clutter_event_get_type (void) G_GNUC_CONST;

gboolean                clutter_events_pending                  (void);
ClutterEvent *          clutter_event_get                       (void);
ClutterEvent *          clutter_event_peek                      (void);
void                    clutter_event_put                       (const ClutterEvent     *event);

ClutterEvent *          clutter_event_new                       (ClutterEventType        type);
ClutterEvent *          clutter_event_copy                      (const ClutterEvent     *event);
void                    clutter_event_free                      (ClutterEvent           *event);

ClutterEventType        clutter_event_type                      (const ClutterEvent     *event);
void                    clutter_event_set_flags                 (ClutterEvent           *event,
                                                                 ClutterEventFlags       flags);
ClutterEventFlags       clutter_event_get_flags                 (const ClutterEvent     *event);
void                    clutter_event_set_time                  (ClutterEvent           *event,
                                                                 guint32                 time_);
guint32                 clutter_event_get_time                  (const ClutterEvent     *event);
void                    clutter_event_set_state                 (ClutterEvent           *event,
                                                                 ClutterModifierType     state);
ClutterModifierType     clutter_event_get_state                 (const ClutterEvent     *event);
void                    clutter_event_set_device                (ClutterEvent           *event,
                                                                 ClutterInputDevice     *device);
ClutterInputDevice *    clutter_event_get_device                (const ClutterEvent     *event);
void                    clutter_event_set_source_device         (ClutterEvent           *event,
                                                                 ClutterInputDevice     *device);

ClutterInputDevice *    clutter_event_get_source_device         (const ClutterEvent     *event);
void                    clutter_event_set_source                (ClutterEvent           *event,
                                                                 ClutterActor           *actor);
ClutterActor *          clutter_event_get_source                (const ClutterEvent     *event);
void                    clutter_event_set_stage                 (ClutterEvent           *event,
                                                                 ClutterStage           *stage);
ClutterStage *          clutter_event_get_stage                 (const ClutterEvent     *event);

gint                    clutter_event_get_device_id             (const ClutterEvent     *event);
ClutterInputDeviceType  clutter_event_get_device_type           (const ClutterEvent     *event);

void                    clutter_event_set_coords                (ClutterEvent           *event,
                                                                 gfloat                  x,
                                                                 gfloat                  y);
void                    clutter_event_get_coords                (const ClutterEvent     *event,
                                                                 gfloat                 *x,
                                                                 gfloat                 *y);

gdouble *               clutter_event_get_axes                  (const ClutterEvent     *event,
                                                                 guint                  *n_axes);

void                    clutter_event_set_key_symbol            (ClutterEvent           *event,
                                                                 guint                   key_sym);
guint                   clutter_event_get_key_symbol            (const ClutterEvent     *event);
void                    clutter_event_set_key_code              (ClutterEvent           *event,
                                                                 guint16                 key_code);
guint16                 clutter_event_get_key_code              (const ClutterEvent     *event);
void                    clutter_event_set_key_unicode           (ClutterEvent           *event,
                                                                 guint32                 key_unicode);
guint32                 clutter_event_get_key_unicode           (const ClutterEvent     *event);

void                    clutter_event_set_button                (ClutterEvent           *event,
                                                                 guint32                 button);
guint32                 clutter_event_get_button                (const ClutterEvent     *event);
guint                   clutter_event_get_click_count           (const ClutterEvent     *event);

void                    clutter_event_set_related               (ClutterEvent           *event,
                                                                 ClutterActor           *actor);
ClutterActor *          clutter_event_get_related               (const ClutterEvent     *event);

void                    clutter_event_set_scroll_direction      (ClutterEvent           *event,
                                                                 ClutterScrollDirection  direction);
ClutterScrollDirection  clutter_event_get_scroll_direction      (const ClutterEvent     *event);

guint32                 clutter_keysym_to_unicode               (guint                   keyval);
guint                   clutter_unicode_to_keysym               (guint32                 wc);

guint32                 clutter_get_current_event_time          (void);
const ClutterEvent *    clutter_get_current_event               (void);

G_END_DECLS

#endif /* __CLUTTER_EVENT_H__ */
