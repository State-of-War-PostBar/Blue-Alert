/*************************************************************************************************
*                                                                                                *
*                                         [ Blue Alert ]                                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                              A free, open-source indie RTS game.                               *
*               (ɔ) 2017 - 2023 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*           Blue Alert is a free software. You can freely do whatever you want with it           *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                  published by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.                  *
*                                                                                                *
*            Blue Alert is created, intended to be useful, but without any warranty.             *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*                 You should have received a copy of the license along with the                  *
*                        source code of this program. If not, please see                         *
*           <https://github.com/State-of-War-PostBar/Blue-Alert/blob/master/LICENSE>.            *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                     <https://github.com/State-of-War-PostBar/Blue-Alert>.                      *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#include "brm.h"

#include "../container/string.h"
#include "../memory/heap_memory.h"

static
void
blrt_FreeBrmData( void *data )
{
    blrt_String_DestroyS(&(((blrt_BrmData *)data)->data));
}

static
inline
blrt_BrmData
blrt_BrmData_Gen( void )
{
    return (blrt_BrmData)
    {
        .data = blrt_String_CreateS()
    };
}

typedef enum blrt_Brm_ParsingState
{
    /// Ready to read a token.
    BLRT_BRM_READY            = 1,
    /// Currently in a token.
    BLRT_BRM_IN_TOKEN         = 1 << 1,
    /// Ate a token and currently looking for next identifier.
    BLRT_BRM_TOKEN_EATEN      = 1 << 2,
    /// Ate a token and a = and currently looking for value for the pair.
    BLRT_BRM_AWAIT_ASSIGNMENT = 1 << 3,
    /// In comment section, everything is ignored.
    BLRT_BRM_COMMENT          = 1 << 4,
    /// A starting /, maybe a comment section later on.
    BLRT_BRM_POSSIBLE_COMMENT = 1 << 5,
    /// A [ passed through, loading block name for the block.
    BLRT_BRM_IN_BLOCK_NAME    = 1 << 6,
    /// A @ escape character.
    BLRT_BRM_ESCAPE = 1 << 7,
    /// In raw string section.
    BLRT_BRM_RAW_STRING       = 1 << 8,
    /// Invalid syntax in block name, everything is discarded.
    BLRT_BRM_DISCARD          = 1 << 9,
    /// After = and possible spaces, start assigning process.
    BLRT_BRM_ASSIGNING        = 1 << 10,
} blrt_Brm_ParsingState;

blrt_Brm
blrt_Brm_Load( const char *str )
{
    blrt_Brm brm =
    {
        .contents = blrt_RadixTree_CreateS(blrt_FreeBrmData)
    };
    if (!str)
        return brm;

    blrt_Brm_ParsingState state = BLRT_BRM_READY;
    blrt_String token = blrt_String_CreateS(),
                buffer_key = blrt_String_CreateS(),
                buffer_block_name = blrt_String_CreateS();
    blrt_String_ExpandUntilOnce(&token, 16ULL);
    blrt_String_ExpandUntilOnce(&buffer_key, 16ULL);
    blrt_String_ExpandUntilOnce(&buffer_block_name, 8ULL);

    for (char ch = *str; ch; ch = *++str)
    {
        switch (ch)
        {
            case ('\r'):
            {
                // Carriage return is always ignored.
                state &= ~BLRT_BRM_POSSIBLE_COMMENT;
                break;
            }
            case ('\"'):
            {
                state &= ~BLRT_BRM_POSSIBLE_COMMENT;
                if ((state & BLRT_BRM_COMMENT) || (state & BLRT_BRM_DISCARD))
                    break;
                if (state & BLRT_BRM_RAW_STRING)
                {
                    // End of raw string section.
                    state &= ~BLRT_BRM_RAW_STRING;
                    break;
                }
                if (state & BLRT_BRM_ESCAPE)
                {
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_BRM_ESCAPE;
                    break;
                }
                // Start of raw string section.
                state |= BLRT_BRM_RAW_STRING;
                if (state & BLRT_BRM_AWAIT_ASSIGNMENT)
                {
                    // If it's waiting for assignment, start the assignment.
                    state &= ~BLRT_BRM_AWAIT_ASSIGNMENT;
                    state |= BLRT_BRM_ASSIGNING;
                    break;
                }
                if (state & BLRT_BRM_TOKEN_EATEN)
                {
                    // Raw string after eaten token and a space means that last token is a flag.
                    blrt_BrmData node = blrt_BrmData_Gen();
                    node.data_type = BLRT_BRM_FLAG;
                    if (buffer_block_name.length)
                        blrt_String_PushFrontS(&token, buffer_block_name.ptr);
                    blrt_RadixTree_Insert(&(brm.contents), token.ptr, sizeof(blrt_BrmData), &node);
                    blrt_String_Clear(&token);
                    state &= ~BLRT_BRM_TOKEN_EATEN;
                    state |= BLRT_BRM_IN_TOKEN;
                    break;
                }
                if (state & BLRT_BRM_IN_BLOCK_NAME)
                {
                    // WTF? Raw string in block names?
                    // Well, it's allowed anyways as long as there are no spaces.
                    break;
                }
                if ((state & BLRT_BRM_IN_TOKEN) || (state & BLRT_BRM_ASSIGNING))
                {
                    // Conjunction with raw string is allowed.
                    break;
                }
                if (state & BLRT_BRM_READY)
                {
                    // Cancel ready state.
                    state &= ~BLRT_BRM_READY;
                    state |= BLRT_BRM_IN_TOKEN;
                    break;
                }
                break;
            }
            case ('@'):
            {
                state &= ~BLRT_BRM_POSSIBLE_COMMENT;
                if ((state & BLRT_BRM_COMMENT) || (state & BLRT_BRM_DISCARD))
                    break;
                if (state & BLRT_BRM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if (state & BLRT_BRM_ESCAPE)
                {
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_BRM_ESCAPE;
                    break;
                }
                // Mark next character as escaped.
                state |= BLRT_BRM_ESCAPE;
                if (state & BLRT_BRM_AWAIT_ASSIGNMENT)
                {
                    // If it's waiting for assignment, start the assignment.
                    state &= ~BLRT_BRM_AWAIT_ASSIGNMENT;
                    state |= BLRT_BRM_ASSIGNING;
                    break;
                }
                if (state & BLRT_BRM_TOKEN_EATEN)
                {
                    // @ after eaten token and a space means that last token is a flag.
                    blrt_BrmData node = blrt_BrmData_Gen();
                    node.data_type = BLRT_BRM_FLAG;
                    if (buffer_block_name.length)
                        blrt_String_PushFrontS(&token, buffer_block_name.ptr);
                    blrt_RadixTree_Insert(&(brm.contents), token.ptr, sizeof(blrt_BrmData), &node);
                    blrt_String_Clear(&token);
                    state &= ~BLRT_BRM_TOKEN_EATEN;
                    state |= BLRT_BRM_IN_TOKEN;
                    break;
                }
                if (state & BLRT_BRM_IN_BLOCK_NAME)
                {
                    break;
                }
                if ((state & BLRT_BRM_IN_TOKEN) || (state & BLRT_BRM_ASSIGNING))
                {
                    break;
                }
                if (state & BLRT_BRM_READY)
                {
                    // Cancel ready state.
                    state &= ~BLRT_BRM_READY;
                    state |= BLRT_BRM_IN_TOKEN;
                    break;
                }
                break;
            }
            case (' '):
            case ('\t'):
            {
                state &= ~BLRT_BRM_POSSIBLE_COMMENT;
                if ((state & BLRT_BRM_COMMENT) || (state & BLRT_BRM_DISCARD))
                    break;
                if (state & BLRT_BRM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if (state & BLRT_BRM_ESCAPE)
                {
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_BRM_ESCAPE;
                    break;
                }
                if (state & BLRT_BRM_IN_BLOCK_NAME)
                {
                    // Weird syntax like [abc.e      fdvnj]
                    // Everything after e and before ] will be discarded.
                    state |= BLRT_BRM_DISCARD;
                    break;
                }
                if (state & BLRT_BRM_AWAIT_ASSIGNMENT)
                {
                    // Space after =, ignored.
                    break;
                }
                if (state & BLRT_BRM_ASSIGNING)
                {
                    // Assignment finished, write values.
                    blrt_BrmData node = blrt_BrmData_Gen();
                    node.data_type = BLRT_BRM_PAIR;
                    blrt_String_PushS(&(node.data), token.ptr);
                    if (buffer_block_name.length)
                        blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                    blrt_RadixTree_Insert(&(brm.contents), buffer_key.ptr, sizeof(blrt_BrmData), &node);

                    blrt_String_Clear(&buffer_key);
                    blrt_String_Clear(&token);
                    state &= ~BLRT_BRM_ASSIGNING;
                    state |= BLRT_BRM_READY;
                    break;
                }
                if (state & BLRT_BRM_IN_TOKEN)
                {
                    // Token awaits assignment.
                    state &= ~BLRT_BRM_IN_TOKEN;
                    state |= BLRT_BRM_TOKEN_EATEN;
                    break;
                }
                if (state & BLRT_BRM_READY)
                {
                    // Do nothing.
                    break;
                }
                break;
            }
            case ('\n'):
            {
                state &= ~BLRT_BRM_POSSIBLE_COMMENT;
                if ((state & BLRT_BRM_RAW_STRING) || (state & BLRT_BRM_ESCAPE))
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if (state & BLRT_BRM_DISCARD)
                {
                    break;
                }
                if (state & BLRT_BRM_COMMENT)
                {
                    // Stop the line of comment.
                    state &= ~BLRT_BRM_COMMENT;
                    break;
                }
                if (state & BLRT_BRM_ESCAPE)
                {
                    // Still escape the character.
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_BRM_ESCAPE;
                    break;
                }
                if (state & BLRT_BRM_IN_BLOCK_NAME)
                {
                    // Weird syntax like [abc.e     
                    //                      fdvnj]
                    // Everything after e and before ] will be ignored.
                    state |= BLRT_BRM_DISCARD;
                    break;
                }
                if (state & BLRT_BRM_AWAIT_ASSIGNMENT)
                {
                    // Space after =. ignored.
                    break;
                }
                if (state & BLRT_BRM_ASSIGNING)
                {
                    // Assignment finished, write values.
                    blrt_BrmData node = blrt_BrmData_Gen();
                    node.data_type = BLRT_BRM_PAIR;
                    blrt_String_PushS(&(node.data), token.ptr);
                    if (buffer_block_name.length)
                        blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                    blrt_RadixTree_Insert(&(brm.contents), buffer_key.ptr, sizeof(blrt_BrmData), &node);

                    blrt_String_Clear(&buffer_key);
                    blrt_String_Clear(&token);
                    state &= ~BLRT_BRM_ASSIGNING;
                    state |= BLRT_BRM_READY;
                    break;
                }
                if (state & BLRT_BRM_IN_TOKEN)
                {
                    // Token eaten.
                    state &= ~BLRT_BRM_IN_TOKEN;
                    state |= BLRT_BRM_TOKEN_EATEN;
                    break;
                }
                if (state & BLRT_BRM_READY)
                {
                    // Do nothing.
                    break;
                }
                break;
            }
            case ('#'):
            {
                state &= ~BLRT_BRM_POSSIBLE_COMMENT;
                if (state & BLRT_BRM_COMMENT)
                    break;
                if (state & BLRT_BRM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if (state & BLRT_BRM_ESCAPE)
                {
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_BRM_ESCAPE;
                    break;
                }
                // Start comment section immediately.
                state |= BLRT_BRM_COMMENT;
                break;
            }
            case ('/'):
            {
                if (state & BLRT_BRM_COMMENT)
                    break;
                if (state & BLRT_BRM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if (state & BLRT_BRM_ESCAPE)
                {
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_BRM_ESCAPE;
                    break;
                }
                if (state & BLRT_BRM_POSSIBLE_COMMENT)
                {
                    // If the possible comment flag is set, clear out all flags.
                    // and begin comment section.
                    state &= ~BLRT_BRM_POSSIBLE_COMMENT;
                    state |= BLRT_BRM_COMMENT;
                    break;
                }
                else
                {
                    // Otherwise, mark a possible comment flag.
                    state |= BLRT_BRM_POSSIBLE_COMMENT;
                    break;
                }
                break;
            }
            case ('['):
            {
                state &= ~BLRT_BRM_POSSIBLE_COMMENT;
                if ((state & BLRT_BRM_COMMENT) || (state & BLRT_BRM_DISCARD))
                    break;
                if (state & BLRT_BRM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if (state & BLRT_BRM_ESCAPE)
                {
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_BRM_ESCAPE;
                    break;
                }
                state |= BLRT_BRM_IN_BLOCK_NAME;
                if ((state & BLRT_BRM_READY) ||
                    (state & BLRT_BRM_IN_TOKEN) ||
                    (state & BLRT_BRM_TOKEN_EATEN) ||
                    (state & BLRT_BRM_AWAIT_ASSIGNMENT) ||
                    (state & BLRT_BRM_ASSIGNING))
                {
                    // Block starting. Process previous flag/assignment.
                    if (state & BLRT_BRM_AWAIT_ASSIGNMENT)
                    {
                        // Block start after a = without value, treat the key as flag.
                        blrt_BrmData node = blrt_BrmData_Gen();
                        node.data_type = BLRT_BRM_FLAG;
                        if (buffer_block_name.length)
                            blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                        blrt_RadixTree_Insert(&(brm.contents), buffer_key.ptr, sizeof(blrt_BrmData), &node);
                        blrt_String_Clear(&token);
                        blrt_String_Clear(&buffer_block_name);
                        state &= ~BLRT_BRM_AWAIT_ASSIGNMENT;
                        break;
                    }
                    if (state & BLRT_BRM_ASSIGNING)
                    {
                        // End current assigning.
                        blrt_BrmData node = blrt_BrmData_Gen();
                        node.data_type = BLRT_BRM_PAIR;
                        blrt_String_PushS(&(node.data), token.ptr);
                        if (buffer_block_name.length)
                            blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                        blrt_RadixTree_Insert(&(brm.contents), buffer_key.ptr, sizeof(blrt_BrmData), &node);
                        blrt_String_Clear(&buffer_key);
                        blrt_String_Clear(&token);
                        blrt_String_Clear(&buffer_block_name);
                        state &= ~BLRT_BRM_ASSIGNING;
                        state &= ~BLRT_BRM_IN_TOKEN;
                        break;
                    }
                    if ((state & BLRT_BRM_IN_TOKEN) || (state & BLRT_BRM_TOKEN_EATEN))
                    {
                        // Block start right after previous token, treat it as flag.
                        blrt_BrmData node = blrt_BrmData_Gen();
                        node.data_type = BLRT_BRM_FLAG;
                        if (buffer_block_name.length)
                            blrt_String_PushFrontS(&token, buffer_block_name.ptr);
                        blrt_RadixTree_Insert(&(brm.contents), token.ptr, sizeof(blrt_BrmData), &node);
                        blrt_String_Clear(&token);
                        blrt_String_Clear(&buffer_block_name);
                        state &= ~BLRT_BRM_IN_TOKEN;
                        state &= ~BLRT_BRM_TOKEN_EATEN;
                        break;
                    }
                }
                break;
            }
            case (']'):
            {
                state &= ~BLRT_BRM_POSSIBLE_COMMENT;
                if (state & BLRT_BRM_COMMENT)
                    break;
                if (state & BLRT_BRM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if (state & BLRT_BRM_ESCAPE)
                {
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_BRM_ESCAPE;
                    break;
                }
                if ((state & BLRT_BRM_IN_BLOCK_NAME) || (state & BLRT_BRM_DISCARD))
                {
                    // End of block, record block name.
                    blrt_String_Clear(&buffer_block_name);
                    if (token.length)
                    {
                        blrt_String_PushS(&buffer_block_name, token.ptr);
                        blrt_String_PushC(&buffer_block_name, '.');
                    }

                    // Clear current token, and ready to record next token.
                    blrt_String_Clear(&token);
                    state = BLRT_BRM_READY;
                    break;
                }
                break;
            }
            case ('='):
            {
                state &= ~BLRT_BRM_POSSIBLE_COMMENT;
                if ((state & BLRT_BRM_COMMENT) || (state & BLRT_BRM_DISCARD))
                    break;
                if (state & BLRT_BRM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if (state & BLRT_BRM_ESCAPE)
                {
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_BRM_ESCAPE;
                    break;
                }
                if ((state & BLRT_BRM_TOKEN_EATEN) || (state & BLRT_BRM_IN_TOKEN))
                {
                    // Token waiting for assignment, start assigning process.
                    blrt_String_PushS(&buffer_key, token.ptr);
                    blrt_String_Clear(&token);
                    state &= ~(BLRT_BRM_TOKEN_EATEN | BLRT_BRM_IN_TOKEN);
                    state |= BLRT_BRM_AWAIT_ASSIGNMENT;
                    break;
                }
                if (state & BLRT_BRM_ASSIGNING)
                {
                    // Syntax error, = sign within assignment.
                    // Assignment ends, and start record new token.
                    if (!token.length)
                    {
                        // Token is empty, treat token before = as flag.
                        blrt_BrmData node = blrt_BrmData_Gen();
                        node.data_type = BLRT_BRM_FLAG;
                        if (buffer_block_name.length)
                            blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                        blrt_RadixTree_Insert(&(brm.contents), buffer_key.ptr, sizeof(blrt_BrmData), &node);
                    }
                    else
                    {
                        blrt_BrmData node = blrt_BrmData_Gen();
                        node.data_type = BLRT_BRM_PAIR;
                        blrt_String_PushS(&(node.data), token.ptr);
                        if (buffer_block_name.length)
                            blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                        blrt_RadixTree_Insert(&(brm.contents), buffer_key.ptr, sizeof(blrt_BrmData), &node);
                    }
                    blrt_String_Clear(&buffer_key);
                    blrt_String_Clear(&token);
                    state &= ~BLRT_BRM_ASSIGNING;
                    state |= BLRT_BRM_READY;
                    break;
                }
                if (state & BLRT_BRM_READY)
                {
                    // = without key, ignored.
                    break;
                }
                break;
            }
            default:
            {
                state &= ~BLRT_BRM_POSSIBLE_COMMENT;
                if ((state & BLRT_BRM_COMMENT) || (state & BLRT_BRM_DISCARD))
                    break;
                if (state & BLRT_BRM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if (state & BLRT_BRM_ESCAPE)
                {
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_BRM_ESCAPE;
                    break;
                }
                if (state & BLRT_BRM_IN_BLOCK_NAME)
                {
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_BRM_TOKEN_EATEN;
                    break;
                }
                if (state & BLRT_BRM_AWAIT_ASSIGNMENT)
                {
                    // Start the assignment.
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_BRM_AWAIT_ASSIGNMENT;
                    state |= BLRT_BRM_ASSIGNING;
                    break;
                }
                if (state & BLRT_BRM_TOKEN_EATEN)
                {
                    // An eaten token means that the previous token is a flag.

                    // Record the flag
                    blrt_BrmData node = blrt_BrmData_Gen();
                    node.data_type = BLRT_BRM_FLAG;
                    if (buffer_block_name.length)
                        blrt_String_PushFrontS(&token, buffer_block_name.ptr);
                    blrt_RadixTree_Insert(&(brm.contents), token.ptr, sizeof(blrt_BrmData), &node);

                    // Clear current token and start a new recording.
                    blrt_String_Clear(&token);
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_BRM_TOKEN_EATEN;
                    state |= BLRT_BRM_IN_TOKEN;
                    break;
                }
                if (state & BLRT_BRM_READY)
                {
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_BRM_READY;
                    state |= BLRT_BRM_IN_TOKEN;
                    break;
                }
                blrt_String_PushC(&token, ch);
                break;
            }
        }
    }

    // Loading ends, evaluate left over tokens
    if (state & BLRT_BRM_RAW_STRING)
        // Unresolved raw string, discard everything.
        ;
    else if (state & BLRT_BRM_ESCAPE)
        // Unresolved escaping, discard it.
        ;
    else if (state & BLRT_BRM_AWAIT_ASSIGNMENT)
    {
        // Left over =, treat its key as flag.
        blrt_BrmData node = blrt_BrmData_Gen();
        node.data_type = BLRT_BRM_FLAG;
        if (buffer_block_name.length)
            blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
        blrt_RadixTree_Insert(&(brm.contents), buffer_key.ptr, sizeof(blrt_BrmData), &node);
    }
    else if (state & BLRT_BRM_ASSIGNING)
    {
        // Finish the assignment.
        blrt_BrmData node = blrt_BrmData_Gen();
        node.data_type = BLRT_BRM_PAIR;
        blrt_String_PushS(&(node.data), token.ptr);
        if (buffer_block_name.length)
            blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
        blrt_RadixTree_Insert(&(brm.contents), buffer_key.ptr, sizeof(blrt_BrmData), &node);
    }
    else if ((state & BLRT_BRM_IN_TOKEN) || (state & BLRT_BRM_TOKEN_EATEN))
    {
        // Left over token, treat as flag.
        blrt_BrmData node = blrt_BrmData_Gen();
        node.data_type = BLRT_BRM_FLAG;
        if (buffer_block_name.length)
            blrt_String_PushFrontS(&token, buffer_block_name.ptr);
        blrt_RadixTree_Insert(&(brm.contents), token.ptr, sizeof(blrt_BrmData), &node);
    }

    blrt_String_DestroyS(&token);
    blrt_String_DestroyS(&buffer_key),
    blrt_String_DestroyS(&buffer_block_name);
    return brm;
}

void
blrt_Brm_Destroy( blrt_Brm *brm )
{
    blrt_RadixTree_DestroyS(&(brm->contents));
}

bool
blrt_Brm_HasFlag( const blrt_Brm *brm, const char *flag )
{
    blrt_RadixTreeNode *node = blrt_RadixTree_Get(&(brm->contents), flag);
    if (!node || !node->data)
        return false;
    return ((blrt_BrmData *)(node->data))->data_type == BLRT_BRM_FLAG;
}

char *
blrt_Brm_GetValue( const blrt_Brm *brm, const char *key )
{
    blrt_RadixTreeNode *node = blrt_RadixTree_Get(&(brm->contents), key);
    if (!node || !node->data)
        return NULL;
    return ((blrt_BrmData *)(node->data))->data.ptr;
}
