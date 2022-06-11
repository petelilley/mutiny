#ifndef __MT_ARGS_H__
#define __MT_ARGS_H__

struct _mt_settings;

/**
 * @brief Parses and interperts the compiler's command-line invocation.
 *
 * @param args Number of command-line arguments.
 * @param argv Array of command-line arguments.
 *
 * @return Generated runtime settings for the compiler.
 *
 * @see mt_settings_t
 */
struct _mt_settings* mt_args_decode(unsigned argc, char* const* argv);

#endif // __MT_ARGS_H__
