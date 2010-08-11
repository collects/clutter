#include <clutter/clutter.h>

#include <errno.h>
#include <stdlib.h>
#include <glib.h>
#include <gmodule.h>

typedef struct
{
  char *name;
  char *source;
} ShaderSource;

/* These variables are used instead of the standard GLSL variables on
   GLES 2 */
#ifdef COGL_HAS_GLES2

#define GLES2_VARS \
  "precision mediump float;\n" \
  "varying vec2 tex_coord[1];\n" \
  "varying vec4 frag_color;\n"
#define TEX_COORD "tex_coord[0]"
#define COLOR_VAR "frag_color"

#else /* COGL_HAS_GLES2 */

#define GLES2_VARS ""
#define TEX_COORD "gl_TexCoord[0]"
#define COLOR_VAR "gl_Color"

#endif /* COGL_HAS_GLES2 */

/* a couple of boilerplate defines that are common amongst all the
 * sample shaders
 */

/* FRAGMENT_SHADER_BEGIN: generate boilerplate with a local vec4 color already
 * initialized, from a sampler2D in a variable tex.
 */
#define FRAGMENT_SHADER_VARS					\
  GLES2_VARS                                                    \
  "uniform sampler2D tex;"					\
  "uniform float x_step, y_step;"				\

#define FRAGMENT_SHADER_BEGIN					\
  "void main (){"						\
  "  vec4 color = texture2D (tex, vec2(" TEX_COORD "));"

/* FRAGMENT_SHADER_END: apply the changed color to the output buffer correctly
 * blended with the gl specified color (makes the opacity of actors work
 * correctly).
 */
#define FRAGMENT_SHADER_END                    \
      "  gl_FragColor = color;"    \
      "  gl_FragColor = gl_FragColor * " COLOR_VAR ";" \
      "}"

static ShaderSource shaders[]=
  {
    {"brightness-contrast",
     FRAGMENT_SHADER_VARS
     "uniform float brightness, contrast;"
     FRAGMENT_SHADER_BEGIN
     " color.rgb /= color.a;"
     " color.rgb = (color.rgb - vec3(0.5, 0.5, 0.5)) * contrast + "
          "vec3 (brightness + 0.5, brightness + 0.5, brightness + 0.5);"
     " color.rgb *= color.a;"
     FRAGMENT_SHADER_END
    },

    {"box-blur",
     FRAGMENT_SHADER_VARS

#if GPU_SUPPORTS_DYNAMIC_BRANCHING
     "uniform float radius;"
     FRAGMENT_SHADER_BEGIN
     "float u, v;"
     "int count = 1;"
     "for (u=-radius;u<radius;u++)"
     "  for (v=-radius;v<radius;v++)"
     "    {"
     "      color += texture2D(tex, "
     "          vec2(" TEX_COORD ".s + u * 2.0 * x_step, "
     "               " TEX_COORD ".t + v * 2.0 * y_step));"
     "      count ++;"
     "    }"
     "color = color / float(count);"
     FRAGMENT_SHADER_END
#else
     "vec4 get_rgba_rel(sampler2D tex, float dx, float dy)"
     "{"
     "  return texture2D (tex, " TEX_COORD ".st "
     "                         + vec2(dx, dy) * 2.0);"
     "}"

     FRAGMENT_SHADER_BEGIN
     "  float count = 1.0;"
     "  color += get_rgba_rel (tex, -x_step, -y_step); count++;"
     "  color += get_rgba_rel (tex, -x_step,  0.0);    count++;"
     "  color += get_rgba_rel (tex, -x_step,  y_step); count++;"
     "  color += get_rgba_rel (tex,  0.0,    -y_step); count++;"
     "  color += get_rgba_rel (tex,  0.0,     0.0);    count++;"
     "  color += get_rgba_rel (tex,  0.0,     y_step); count++;"
     "  color += get_rgba_rel (tex,  x_step, -y_step); count++;"
     "  color += get_rgba_rel (tex,  x_step,  0.0);    count++;"
     "  color += get_rgba_rel (tex,  x_step,  y_step); count++;"
     "  color = color / count;"
     FRAGMENT_SHADER_END
#endif
    },

    {"invert",
     FRAGMENT_SHADER_VARS
     FRAGMENT_SHADER_BEGIN
     "  color.rgb /= color.a;"
     "  color.rgb = vec3(1.0, 1.0, 1.0) - color.rgb;\n"
     "  color.rgb *= color.a;"
     FRAGMENT_SHADER_END
    },

    {"brightness-contrast",
     FRAGMENT_SHADER_VARS
     "uniform float brightness;"
     "uniform float contrast;"
     FRAGMENT_SHADER_BEGIN
     "  color.rgb /= color.a;"
     "  color.r = (color.r - 0.5) * contrast + brightness + 0.5;"
     "  color.g = (color.g - 0.5) * contrast + brightness + 0.5;"
     "  color.b = (color.b - 0.5) * contrast + brightness + 0.5;"
     "  color.rgb *= color.a;"
     FRAGMENT_SHADER_END
    },

    {"gray",
     FRAGMENT_SHADER_VARS
     FRAGMENT_SHADER_BEGIN
     "  float avg = (color.r + color.g + color.b) / 3.0;"
     "  color.r = avg;"
     "  color.g = avg;"
     "  color.b = avg;"
     FRAGMENT_SHADER_END
    },

    {"combined-mirror",
     FRAGMENT_SHADER_VARS
     FRAGMENT_SHADER_BEGIN
     "  vec4 colorB = texture2D (tex, vec2(" TEX_COORD ".ts));"
     "  float avg = (color.r + color.g + color.b) / 3.0;"
     "  color.r = avg;"
     "  color.g = avg;"
     "  color.b = avg;"
     "  color = (color + colorB)/2.0;"
     FRAGMENT_SHADER_END
    },

    {"edge-detect",
     FRAGMENT_SHADER_VARS
     "float get_avg_rel(sampler2D texB, float dx, float dy)"
     "{"
     "  vec4 colorB = texture2D (texB, " TEX_COORD ".st + vec2(dx, dy));"
     "  return (colorB.r + colorB.g + colorB.b) / 3.0;"
     "}"
     FRAGMENT_SHADER_BEGIN
     "  mat3 sobel_h = mat3( 1.0,  2.0,  1.0,"
     "                       0.0,  0.0,  0.0,"
     "                      -1.0, -2.0, -1.0);"
     "  mat3 sobel_v = mat3( 1.0,  0.0, -1.0,"
     "                       2.0,  0.0, -2.0,"
     "                       1.0,  0.0, -1.0);"
     "  mat3 map = mat3( get_avg_rel(tex, -x_step, -y_step),"
     "                   get_avg_rel(tex, -x_step, 0.0),"
     "                   get_avg_rel(tex, -x_step, y_step),"
     "                   get_avg_rel(tex, 0.0, -y_step),"
     "                   get_avg_rel(tex, 0.0, 0.0),"
     "                   get_avg_rel(tex, 0.0, y_step),"
     "                   get_avg_rel(tex, x_step, -y_step),"
     "                   get_avg_rel(tex, x_step, 0.0),"
     "                   get_avg_rel(tex, x_step, y_step) );"
     "  mat3 gh = sobel_h * map;"
     "  mat3 gv = map * sobel_v;"
     "  float avgh = (gh[0][0] + gh[0][1] + gh[0][2] +"
     "                gh[1][0] + gh[1][1] + gh[1][2] +"
     "                gh[2][0] + gh[2][1] + gh[2][2]) / 18.0 + 0.5;"
     "  float avgv = (gv[0][0] + gv[0][1] + gv[0][2] +"
     "                gv[1][0] + gv[1][1] + gv[1][2] +"
     "                gv[2][0] + gv[2][1] + gv[2][2]) / 18.0 + 0.5;"
     "  float avg = (avgh + avgv) / 2.0;"
     "  color.r = avg * color.r;"
     "  color.g = avg * color.g;"
     "  color.b = avg * color.b;"
     FRAGMENT_SHADER_END
    }
};

static CoglHandle redhand;
static CoglMaterial *material;
static unsigned int timeout_id = 0;
static int shader_no = 0;

static void
paint_cb (ClutterActor *actor)
{
  int stage_width = clutter_actor_get_width (actor);
  int stage_height = clutter_actor_get_height (actor);
  int image_width = cogl_texture_get_width (redhand);
  int image_height = cogl_texture_get_height (redhand);

  cogl_set_source (material);
  cogl_rectangle (stage_width/2.0f - image_width/2.0f,
                  stage_height/2.0f - image_height/2.0f,
                  stage_width/2.0f + image_width/2.0f,
                  stage_height/2.0f + image_height/2.0f);
}

static void
set_shader_num (int new_no)
{
  CoglHandle shader;
  CoglHandle program;
  int image_width = cogl_texture_get_width (redhand);
  int image_height = cogl_texture_get_height (redhand);
  int uniform_no;

  g_print ("setting shaders[%i] named '%s'\n",
           new_no,
           shaders[new_no].name);

  shader = cogl_create_shader (COGL_SHADER_TYPE_FRAGMENT);
  cogl_shader_source (shader, shaders[new_no].source);
  cogl_shader_compile (shader);

  program = cogl_create_program ();
  cogl_program_attach_shader (program, shader);
  cogl_handle_unref (shader);
  cogl_program_link (program);

  uniform_no = cogl_program_get_uniform_location (program, "tex");
  cogl_program_set_uniform_1i (program, uniform_no, 0);
  uniform_no = cogl_program_get_uniform_location (program, "radius");
  cogl_program_set_uniform_1f (program, uniform_no, 3.0);
  uniform_no = cogl_program_get_uniform_location (program, "brightness");
  cogl_program_set_uniform_1f (program, uniform_no, 0.4);
  uniform_no = cogl_program_get_uniform_location (program, "contrast");
  cogl_program_set_uniform_1f (program, uniform_no, -1.9);

  uniform_no = cogl_program_get_uniform_location (program, "x_step");
  cogl_program_set_uniform_1f (program, uniform_no, 1.0f / image_width);
  uniform_no = cogl_program_get_uniform_location (program, "y_step");
  cogl_program_set_uniform_1f (program, uniform_no, 1.0f / image_height);

  cogl_material_set_user_program (material, program);
  cogl_handle_unref (program);

  shader_no = new_no;
}

static gboolean
button_release_cb (ClutterActor *actor,
                   ClutterEvent *event,
                   void *data)
{
  int new_no;

  /* Stop the automatic cycling if the user want to manually control
   * which shader to display */
  if (timeout_id)
    {
      g_source_remove (timeout_id);
      timeout_id = 0;
    }

  if (event->button.button == 1)
    {
      new_no = shader_no - 1;
      if (new_no < 0)
        new_no = G_N_ELEMENTS (shaders) - 1;
    }
  else
    {
      new_no = shader_no + 1;
      if (new_no >= G_N_ELEMENTS (shaders))
        new_no = 0;
    }

  set_shader_num (new_no);

  return FALSE;
}

static gboolean
key_release_cb (ClutterActor *actor,
                ClutterEvent *event,
                void *user_data)
{
  if (event->key.keyval == CLUTTER_Q || event->key.keyval == CLUTTER_q)
    clutter_main_quit ();

  return FALSE;
}

static gboolean
timeout_cb (void *user_data)
{
  shader_no++;
  if (shader_no > (G_N_ELEMENTS (shaders) - 1))
    shader_no = 0;

  set_shader_num (shader_no);

  return TRUE;
}

static gboolean
idle_cb (void *data)
{
  clutter_actor_queue_redraw (data);

  return TRUE;
}

static gboolean
destroy_window_cb (ClutterStage *stage,
                   ClutterEvent *event,
                   void *user_data)
{
  clutter_main_quit ();
  return TRUE;
}

G_MODULE_EXPORT int
test_cogl_shader_glsl_main (int argc, char *argv[])
{
  ClutterActor *stage;
  char *file;
  GError *error;
  ClutterColor stage_color = { 0x61, 0x64, 0x8c, 0xff };

  clutter_init (&argc, &argv);

  stage = clutter_stage_new ();

  clutter_stage_set_title (CLUTTER_STAGE (stage), "Assembly Shader Test");
  clutter_stage_set_color (CLUTTER_STAGE (stage), &stage_color);

  file = g_build_filename (TESTS_DATADIR, "redhand.png", NULL);
  error = NULL;
  redhand = cogl_texture_new_from_file (file, 0, COGL_PIXEL_FORMAT_ANY,
                                        &error);
  if (redhand == COGL_INVALID_HANDLE)
    g_error ("image load failed: %s", error->message);

  material = cogl_material_new ();
  cogl_material_set_layer (material, 0, redhand);

  set_shader_num (0);
  g_signal_connect_after (stage, "paint", G_CALLBACK (paint_cb), NULL);

  clutter_actor_set_reactive (stage, TRUE);
  g_signal_connect (stage, "button-release-event",
                    G_CALLBACK (button_release_cb), NULL);
  g_signal_connect (stage, "key-release-event",
                    G_CALLBACK (key_release_cb), NULL);

  g_signal_connect (stage, "delete-event",
                    G_CALLBACK (destroy_window_cb), NULL);

  timeout_id = g_timeout_add (1000, timeout_cb, NULL);

  g_idle_add (idle_cb, stage);

  clutter_actor_show_all (stage);

  clutter_main ();

  return EXIT_SUCCESS;
}

