#
#  This file was added by Duzy Chan <code@duzy.info> on 2011-09-30
#
$(call sm-new-module, clutter, shared, gcc)

sm.this.sources := \
  clutter-action.c		\
  clutter-actor-box.c		\
  clutter-actor-meta.c		\
  clutter-actor.c		\
  clutter-align-constraint.c	\
  clutter-alpha.c 		\
  clutter-animatable.c		\
  clutter-animation.c		\
  clutter-animator.c		\
  clutter-backend.c		\
  clutter-behaviour.c 		\
  clutter-behaviour-depth.c	\
  clutter-behaviour-ellipse.c	\
  clutter-behaviour-opacity.c	\
  clutter-behaviour-path.c	\
  clutter-behaviour-rotate.c	\
  clutter-behaviour-scale.c	\
  clutter-bezier.c		\
  clutter-bind-constraint.c	\
  clutter-binding-pool.c	\
  clutter-bin-layout.c		\
  clutter-blur-effect.c		\
  clutter-box.c			\
  clutter-box-layout.c		\
  clutter-cairo-texture.c       \
  clutter-child-meta.c		\
  clutter-click-action.c	\
  clutter-clone.c		\
  clutter-color.c 		\
  clutter-colorize-effect.c	\
  clutter-constraint.c		\
  clutter-container.c		\
  clutter-deform-effect.c	\
  clutter-desaturate-effect.c	\
  clutter-device-manager.c	\
  clutter-drag-action.c		\
  clutter-drop-action.c		\
  clutter-effect.c		\
  clutter-event.c 		\
  clutter-feature.c 		\
  clutter-fixed.c		\
  clutter-fixed-layout.c	\
  clutter-flatten-effect.c	\
  clutter-flow-layout.c		\
  clutter-frame-source.c	\
  clutter-gesture-action.c 		\
  clutter-group.c 		\
  clutter-input-device.c	\
  clutter-interval.c            \
  clutter-keysyms-table.c	\
  clutter-layout-manager.c	\
  clutter-layout-meta.c		\
  clutter-list-model.c		\
  clutter-main.c 		\
  clutter-master-clock.c	\
  clutter-media.c 		\
  clutter-model.c		\
  clutter-offscreen-effect.c	\
  clutter-page-turn-effect.c	\
  clutter-path-constraint.c	\
  clutter-path.c		\
  clutter-rectangle.c 		\
  clutter-score.c 		\
  clutter-script.c		\
  clutter-script-parser.c	\
  clutter-scriptable.c		\
  clutter-settings.c		\
  clutter-shader.c		\
  clutter-shader-effect.c	\
  clutter-shader-types.c	\
  clutter-swipe-action.c	\
  clutter-snap-constraint.c	\
  clutter-stage.c		\
  clutter-stage-manager.c	\
  clutter-stage-window.c	\
  clutter-state.c		\
  clutter-table-layout.c	\
  clutter-texture.c 		\
  clutter-text.c                \
  clutter-timeline.c 		\
  clutter-timeout-pool.c	\
  clutter-units.c		\
  clutter-util.c 		\
  clutter-paint-volume.c 	\

# private source code; these should not be introspected
sm.this.sources += \
  clutter-event-translator.c	\
  clutter-id-pool.c 		\
  clutter-profile.c		\
  clutter-timeout-interval.c    \

# built sources(generated)
sm.this.sources += \
  clutter-enum-types.c		\
  clutter-marshal.c		\

ifeq ($(SUPPORT_X11),true)
sm.this.sources += \
  x11/clutter-backend-x11.c		\
  x11/clutter-device-manager-core-x11.c	\
  x11/clutter-event-x11.c		\
  x11/clutter-input-device-core-x11.c	\
  x11/clutter-keymap-x11.c		\
  x11/clutter-stage-x11.c		\
  x11/clutter-x11-texture-pixmap.c
sm.this.sources += \
  cogl/clutter-glx-texture-pixmap.c
endif

## Cogl sources
sm.this.sources += \
  cogl/clutter-backend-cogl.c	\
  cogl/clutter-stage-cogl.c	\

ifeq ($(SUPPORT_WIN32),true)
sm.this.sources += \
  win32/clutter-backend-win32.c		\
  win32/clutter-device-manager-win32.c	\
  win32/clutter-event-win32.c		\
  win32/clutter-stage-win32.c
endif

ifeq ($(SUPPORT_EGL),true)
 ###
endif

ifeq ($(SUPPORT_OSX),true)
sm.this.sources += \
  osx/clutter-backend-osx.c	\
  osx/clutter-stage-osx.c

## private sources
sm.this.sources += \
  osx/clutter-event-loop-osx.c	\
  osx/clutter-event-osx.c	\
  osx/clutter-device-manager-osx.c
endif

ifeq ($(SUPPORT_WAYLAND),true)
sm.this.sources += \
  evdev/clutter-xkb-utils.c 		\
  wayland/clutter-backend-wayland.c 	\
  wayland/clutter-stage-wayland.c 	\
  wayland/clutter-event-wayland.c 	\
  wayland/clutter-input-device-wayland.c
endif

sm.this.verbose := true

$(sm-build-this)
