#ifndef __MT_LEXER_COMMENT_H__
#define __MT_LEXER_COMMENT_H__

struct _mt_file;
struct _mt_error_reporter;

/**
 * @brief Skips over a line comment in the file.
 * @param file The file to skip over.
 */
void mt_skip_line_comment(struct _mt_file* file);

/**
 * @brief Skips over a block comment in the file.
 * @param file The file to skip over.
 * @param error_reporter The translation unit's error reporter.
 */
void mt_skip_block_comment(struct _mt_file* file, struct _mt_error_reporter* error_reporter);

#endif // __MT_LEXER_COMMENT_H__