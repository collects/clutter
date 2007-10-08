#ifndef __CLUTTER_SCRIPT_H__
#define __CLUTTER_SCRIPT_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define CLUTTER_TYPE_SCRIPT             (clutter_script_get_type ())
#define CLUTTER_SCRIPT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLUTTER_TYPE_SCRIPT, ClutterScript))
#define CLUTTER_IS_SCRIPT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLUTTER_TYPE_SCRIPT))
#define CLUTTER_SCRIPT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), CLUTTER_TYPE_SCRIPT, ClutterScriptClass))
#define CLUTTER_IS_SCRIPT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), CLUTTER_TYPE_SCRIPT))
#define CLUTTER_SCRIPT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), CLUTTER_TYPE_SCRIPT, ClutterScript))

typedef struct _ClutterScript           ClutterScript;
typedef struct _ClutterScriptPrivate    ClutterScriptPrivate;
typedef struct _ClutterScriptClass      ClutterScriptClass;

/**
 * ClutterScriptError:
 * @CLUTTER_SCRIPT_ERROR_INVALID_VALUE: Invalid value
 *
 * #ClutterScript error enumeration.
 *
 * Since: 0.6
 */
typedef enum {
  CLUTTER_SCRIPT_ERROR_INVALID_TYPE_FUNCTION,
  CLUTTER_SCRIPT_ERROR_INVALID_PROPERTY,
  CLUTTER_SCRIPT_ERROR_INVALID_VALUE
} ClutterScriptError;

#define CLUTTER_SCRIPT_ERROR    (clutter_script_error_quark ())
GQuark clutter_script_error_quark (void);

struct _ClutterScript
{
  /*< private >*/
  GObject parent_instance;

  ClutterScriptPrivate *priv;
};

struct _ClutterScriptClass
{
  /*< private >*/
  GObjectClass parent_class;

  /* padding, for future expansion */
  void (*_clutter_reserved1) (void);
  void (*_clutter_reserved2) (void);
  void (*_clutter_reserved3) (void);
  void (*_clutter_reserved4) (void);
  void (*_clutter_reserved5) (void);
  void (*_clutter_reserved6) (void);
  void (*_clutter_reserved7) (void);
  void (*_clutter_reserved8) (void);
};

GType          clutter_script_get_type       (void) G_GNUC_CONST;

ClutterScript *clutter_script_new            (void);
guint          clutter_script_load_from_file (ClutterScript  *script,
                                              const gchar    *filename,
                                              GError        **error);
guint          clutter_script_load_from_data (ClutterScript  *script,
                                              const gchar    *data,
                                              gsize           length,
                                              GError        **error);
GObject *      clutter_script_get_object     (ClutterScript  *script,
                                              const gchar    *name);
GList *        clutter_script_get_objects    (ClutterScript  *script,
                                              const gchar    *first_name,
                                              ...) G_GNUC_NULL_TERMINATED;

G_END_DECLS

#endif /* __CLUTTER_SCRIPT_H__ */
