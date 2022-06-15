#ifndef __MT_ARGS_H__
#define __MT_ARGS_H__

struct _mt_settings;

/**
 * @brief Parses command line arguments.
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return The settings for the compiler to use.
 */
struct _mt_settings* mt_args_decode(unsigned argc, char* const* argv);

#endif // __MT_ARGS_H__
