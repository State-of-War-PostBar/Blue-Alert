/*************************************************************************************************
*                                                                                                *
*                                         [ Blue Alert ]                                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                              A free, open-source indie RTS game.                               *
*               (ɔ) 2017 - 2022 State of War Baidu Postbar, some rights reserved.                *
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

#include "swm.h"

#include "../container/string.h"
#include "../memory/heap_memory.h"

static
void
blrt_FreeSwmData( void *data )
{
    blrt_String_DestroyS(&(((blrt_SwmData *)data)->data));
}

static
inline
blrt_SwmData
blrt_SwmData_Gen( void )
{
    return (blrt_SwmData)
    {
        .data = blrt_String_CreateS()
    };
}

typedef enum blrt_Swm_ParsingState
{
    /// Ready to read a token.
    BLRT_SWM_READY            = 1,
    /// Currently in a token.
    BLRT_SWM_IN_TOKEN         = 1 << 1,
    /// Ate a token and currently looking for next identifier.
    BLRT_SWM_TOKEN_EATEN      = 1 << 2,
    /// Ate a token and a = and currently looking for value for the pair.
    BLRT_SWM_AWAIT_ASSIGNMENT = 1 << 3,
    /// In comment section, everything is ignored.
    BLRT_SWM_COMMENT          = 1 << 4,
    /// A starting /, maybe a comment section later on.
    BLRT_SWM_POSSIBLE_COMMENT = 1 << 5,
    /// A [ passed through, loading block name for the block.
    BLRT_SWM_IN_BLOCK_NAME    = 1 << 6,
    /// In raw string section.
    BLRT_SWM_RAW_STRING       = 1 << 7,
    /// Invalid syntax in block name, everything is discarded.
    BLRT_SWM_DISCARD          = 1 << 8,
    /// After = and possible spaces, start assigning process.
    BLRT_SWM_ASSIGNING        = 1 << 9,
} blrt_Swm_ParsingState;

blrt_Swm
blrt_Swm_Load( const char *str )
{
    blrt_Swm swm =
    {
        .contents = blrt_RadixTree_CreateS(blrt_FreeSwmData)
    };
    if (!str)
        return swm;

    blrt_Swm_ParsingState state = BLRT_SWM_READY;
    blrt_String token = blrt_String_CreateS(),
                buffer_key = blrt_String_CreateS(),
                buffer_block_name = blrt_String_CreateS();
    blrt_String_ExpandUntilOnce(&token, 16ULL);
    blrt_String_ExpandUntilOnce(&buffer_key, 16ULL);
    blrt_String_ExpandUntilOnce(&buffer_block_name, 8ULL);
    char ch = *str;

    while (ch)
    {
        switch (ch)
        {
            case ('\r'):
            {
                state &= ~BLRT_SWM_POSSIBLE_COMMENT;
                // Carriage return is always ignored.
                break;
            }
            case ('\"'):
            {
                state &= ~BLRT_SWM_POSSIBLE_COMMENT;
                if ((state & BLRT_SWM_COMMENT) || (state & BLRT_SWM_DISCARD))
                    break;
                if (state & BLRT_SWM_RAW_STRING)
                {
                    // End of raw string section.
                    state &= ~BLRT_SWM_RAW_STRING;
                    break;
                }
                // Start of raw string section.
                state |= BLRT_SWM_RAW_STRING;
                if (state & BLRT_SWM_AWAIT_ASSIGNMENT)
                {
                    // If it's waiting for assignment, start the assignment.
                    state &= ~BLRT_SWM_AWAIT_ASSIGNMENT;
                    state |= BLRT_SWM_ASSIGNING;
                    break;
                }
                if (state & BLRT_SWM_TOKEN_EATEN)
                {
                    // Raw string after eaten token and a space means that last token is a flag.
                    state &= ~BLRT_SWM_TOKEN_EATEN;
                    state |= BLRT_SWM_IN_TOKEN;
                    blrt_SwmData node = blrt_SwmData_Gen();
                    node.data_type = BLRT_SWM_FLAG;
                    if (buffer_block_name.length)
                        blrt_String_PushFrontS(&token, buffer_block_name.ptr);
                    blrt_RadixTree_Insert(&(swm.contents), token.ptr, sizeof(blrt_SwmData), &node);
                    blrt_String_Clear(&token);
                    break;
                }
                if (state & BLRT_SWM_IN_BLOCK_NAME)
                {
                    // WTF? Raw string in block names?
                    // Well, it's allowed anyways as long as there are no spaces.
                    break;
                }
                if ((state & BLRT_SWM_IN_TOKEN) || (state & BLRT_SWM_ASSIGNING))
                {
                    // Conjunction with raw string is allowed.
                    break;
                }
                if (state & BLRT_SWM_READY)
                {
                    // Cancel ready state.
                    state &= ~BLRT_SWM_READY;
                    state |= BLRT_SWM_IN_TOKEN;
                    break;
                }
                break;
            }
            case (' '):
            case ('\t'):
            {
                state &= ~BLRT_SWM_POSSIBLE_COMMENT;
                if ((state & BLRT_SWM_COMMENT) || (state & BLRT_SWM_DISCARD))
                    break;
                if (state & BLRT_SWM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if (state & BLRT_SWM_IN_BLOCK_NAME)
                {
                    // Weird syntax like [abc.e      fdvnj]
                    // Everything after e and before ] will be discarded.
                    state |= BLRT_SWM_DISCARD;
                    break;
                }
                if (state & BLRT_SWM_AWAIT_ASSIGNMENT)
                {
                    // Space after =, ignored.
                    break;
                }
                if (state & BLRT_SWM_ASSIGNING)
                {
                    // Assignment finished, write values.
                    state &= ~BLRT_SWM_ASSIGNING;
                    state |= BLRT_SWM_READY;
                    blrt_SwmData node = blrt_SwmData_Gen();
                    node.data_type = BLRT_SWM_PAIR;
                    blrt_String_PushS(&(node.data), token.ptr);
                    if (buffer_block_name.length)
                        blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                    blrt_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(blrt_SwmData), &node);

                    blrt_String_Clear(&buffer_key);
                    blrt_String_Clear(&token);
                    break;
                }
                if (state & BLRT_SWM_IN_TOKEN)
                {
                    // Token awaits assignment.
                    state &= ~BLRT_SWM_IN_TOKEN;
                    state |= BLRT_SWM_TOKEN_EATEN;
                    break;
                }
                if (state & BLRT_SWM_READY)
                {
                    // Do nothing.
                    break;
                }
                break;
            }
            case ('\n'):
            {
                state &= ~BLRT_SWM_POSSIBLE_COMMENT;
                if (state & BLRT_SWM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if (state & BLRT_SWM_DISCARD)
                {
                    break;
                }
                if (state & BLRT_SWM_COMMENT)
                {
                    // Stop the line of comment.
                    state &= ~BLRT_SWM_COMMENT;
                    break;
                }
                if (state & BLRT_SWM_IN_BLOCK_NAME)
                {
                    // Weird syntax like [abc.e     
                    //                      fdvnj]
                    // Everything after e and before ] will be ignored.
                    state |= BLRT_SWM_DISCARD;
                    break;
                }
                if (state & BLRT_SWM_AWAIT_ASSIGNMENT)
                {
                    // Space after =. ignored.
                    break;
                }
                if (state & BLRT_SWM_ASSIGNING)
                {
                    // Assignment finished, write values.
                    blrt_SwmData node = blrt_SwmData_Gen();
                    node.data_type = BLRT_SWM_PAIR;
                    blrt_String_PushS(&(node.data), token.ptr);
                    if (buffer_block_name.length)
                        blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                    blrt_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(blrt_SwmData), &node);

                    blrt_String_Clear(&buffer_key);
                    blrt_String_Clear(&token);
                    state &= ~BLRT_SWM_ASSIGNING;
                    state |= BLRT_SWM_READY;
                    break;
                }
                if (state & BLRT_SWM_IN_TOKEN)
                {
                    // Token eaten.
                    state &= ~BLRT_SWM_IN_TOKEN;
                    state |= BLRT_SWM_TOKEN_EATEN;
                    break;
                }
                if (state & BLRT_SWM_READY)
                {
                    // Do nothing.
                    break;
                }
                break;
            }
            case ('#'):
            {
                state &= ~BLRT_SWM_POSSIBLE_COMMENT;
                if (state & BLRT_SWM_COMMENT)
                    break;
                if (state & BLRT_SWM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                // Start comment section immediately.
                state |= BLRT_SWM_COMMENT;
                break;
            }
            case ('/'):
            {
                if (state & BLRT_SWM_COMMENT)
                    break;
                if (state & BLRT_SWM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if (state & BLRT_SWM_POSSIBLE_COMMENT)
                {
                    // If the possible comment flag is set, clear out all flags.
                    // and begin comment section.
                    state &= ~BLRT_SWM_POSSIBLE_COMMENT;
                    state |= BLRT_SWM_COMMENT;
                    break;
                }
                else
                {
                    // Otherwise, mark a possible comment flag.
                    state |= BLRT_SWM_POSSIBLE_COMMENT;
                    break;
                }
                break;
            }
            case ('['):
            {
                state &= ~BLRT_SWM_POSSIBLE_COMMENT;
                if ((state & BLRT_SWM_COMMENT) || (state & BLRT_SWM_DISCARD))
                    break;
                if (state & BLRT_SWM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                state |= BLRT_SWM_IN_BLOCK_NAME;
                if ((state & BLRT_SWM_READY) ||
                    (state & BLRT_SWM_IN_TOKEN) ||
                    (state & BLRT_SWM_TOKEN_EATEN) ||
                    (state & BLRT_SWM_AWAIT_ASSIGNMENT) ||
                    (state & BLRT_SWM_ASSIGNING))
                {
                    // Block starting. Process previous flag/assignment.
                    if (state & BLRT_SWM_AWAIT_ASSIGNMENT)
                    {
                        // Block start after a = without value, treat the key as flag.
                        blrt_SwmData node = blrt_SwmData_Gen();
                        node.data_type = BLRT_SWM_FLAG;
                        if (buffer_block_name.length)
                            blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                        blrt_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(blrt_SwmData), &node);
                        blrt_String_Clear(&token);
                        blrt_String_Clear(&buffer_block_name);
                        state &= ~BLRT_SWM_AWAIT_ASSIGNMENT;
                        break;
                    }
                    if (state & BLRT_SWM_ASSIGNING)
                    {
                        // End current assigning.
                        blrt_SwmData node = blrt_SwmData_Gen();
                        node.data_type = BLRT_SWM_PAIR;
                        blrt_String_PushS(&(node.data), token.ptr);
                        if (buffer_block_name.length)
                            blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                        blrt_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(blrt_SwmData), &node);
                        blrt_String_Clear(&buffer_key);
                        blrt_String_Clear(&token);
                        blrt_String_Clear(&buffer_block_name);
                        state &= ~BLRT_SWM_ASSIGNING;
                        state &= ~BLRT_SWM_IN_TOKEN;
                        break;
                    }
                    if ((state & BLRT_SWM_IN_TOKEN) || (state & BLRT_SWM_TOKEN_EATEN))
                    {
                        // Block start right after previous token, treat it as flag.
                        blrt_SwmData node = blrt_SwmData_Gen();
                        node.data_type = BLRT_SWM_FLAG;
                        if (buffer_block_name.length)
                            blrt_String_PushFrontS(&token, buffer_block_name.ptr);
                        blrt_RadixTree_Insert(&(swm.contents), token.ptr, sizeof(blrt_SwmData), &node);
                        blrt_String_Clear(&token);
                        blrt_String_Clear(&buffer_block_name);
                        state &= ~BLRT_SWM_IN_TOKEN;
                        state &= ~BLRT_SWM_TOKEN_EATEN;
                        break;
                    }
                }
                break;
            }
            case (']'):
            {
                state &= ~BLRT_SWM_POSSIBLE_COMMENT;
                if (state & BLRT_SWM_COMMENT)
                    break;
                if (state & BLRT_SWM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if ((state & BLRT_SWM_IN_BLOCK_NAME) || (state & BLRT_SWM_DISCARD))
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
                    state = BLRT_SWM_READY;
                    break;
                }
                break;
            }
            case ('='):
            {
                state &= ~BLRT_SWM_POSSIBLE_COMMENT;
                if ((state & BLRT_SWM_COMMENT) || (state & BLRT_SWM_DISCARD))
                    break;
                if (state & BLRT_SWM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if ((state & BLRT_SWM_TOKEN_EATEN) || (state & BLRT_SWM_IN_TOKEN))
                {
                    // Token waiting for assignment, start assigning process.
                    state &= ~(BLRT_SWM_TOKEN_EATEN | BLRT_SWM_IN_TOKEN);
                    state |= BLRT_SWM_AWAIT_ASSIGNMENT;
                    blrt_String_PushS(&buffer_key, token.ptr);
                    blrt_String_Clear(&token);
                    break;
                }
                if (state & BLRT_SWM_ASSIGNING)
                {
                    // Syntax error, = sign within assignment.
                    // Assignment ends, and start record new token.
                    if (!token.length)
                    {
                        // Token is empty, treat token before = as flag.
                        blrt_SwmData node = blrt_SwmData_Gen();
                        node.data_type = BLRT_SWM_FLAG;
                        if (buffer_block_name.length)
                            blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                        blrt_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(blrt_SwmData), &node);
                    }
                    else
                    {
                        blrt_SwmData node = blrt_SwmData_Gen();
                        node.data_type = BLRT_SWM_PAIR;
                        blrt_String_PushS(&(node.data), token.ptr);
                        if (buffer_block_name.length)
                            blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                        blrt_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(blrt_SwmData), &node);
                    }
                    state &= ~BLRT_SWM_ASSIGNING;
                    state |= BLRT_SWM_READY;
                    blrt_String_Clear(&buffer_key);
                    blrt_String_Clear(&token);
                    break;
                }
                if (state & BLRT_SWM_READY)
                {
                    // = without key, ignored.
                    break;
                }
                break;
            }
            default:
            {
                state &= ~BLRT_SWM_POSSIBLE_COMMENT;
                if ((state & BLRT_SWM_COMMENT) || (state & BLRT_SWM_DISCARD))
                    break;
                if (state & BLRT_SWM_RAW_STRING)
                {
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if (state & BLRT_SWM_IN_BLOCK_NAME)
                {
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_SWM_TOKEN_EATEN;
                    break;
                }
                if (state & BLRT_SWM_AWAIT_ASSIGNMENT)
                {
                    // Start the assignment.
                    state &= ~BLRT_SWM_AWAIT_ASSIGNMENT;
                    state |= BLRT_SWM_ASSIGNING;
                    blrt_String_PushC(&token, ch);
                    break;
                }
                if (state & BLRT_SWM_TOKEN_EATEN)
                {
                    // An eaten token means that the previous token is a flag.

                    // Record the flag
                    blrt_SwmData node = blrt_SwmData_Gen();
                    node.data_type = BLRT_SWM_FLAG;
                    if (buffer_block_name.length)
                        blrt_String_PushFrontS(&token, buffer_block_name.ptr);
                    blrt_RadixTree_Insert(&(swm.contents), token.ptr, sizeof(blrt_SwmData), &node);

                    // Clear current token and start a new recording.
                    blrt_String_Clear(&token);
                    blrt_String_PushC(&token, ch);
                    state &= ~BLRT_SWM_TOKEN_EATEN;
                    state |= BLRT_SWM_IN_TOKEN;
                    break;
                }
                if (state & BLRT_SWM_READY)
                {
                    state &= ~BLRT_SWM_READY;
                    state |= BLRT_SWM_IN_TOKEN;
                    blrt_String_PushC(&token, ch);
                    break;
                }
                blrt_String_PushC(&token, ch);
                break;
            }
        }
        ch = *++str;
    }

    // Loading got shut down by EOF, evaluate left over tokens
    if (state & BLRT_SWM_RAW_STRING)
        // Unresolved raw string, discard everything.
        ;
    else if (state & BLRT_SWM_AWAIT_ASSIGNMENT)
    {
        // Left over =, treat its key as flag.
        blrt_SwmData node = blrt_SwmData_Gen();
        node.data_type = BLRT_SWM_FLAG;
        if (buffer_block_name.length)
            blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
        blrt_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(blrt_SwmData), &node);
    }
    else if (state & BLRT_SWM_ASSIGNING)
    {
        // Finish the assignment.
        blrt_SwmData node = blrt_SwmData_Gen();
        node.data_type = BLRT_SWM_PAIR;
        blrt_String_PushS(&(node.data), token.ptr);
        if (buffer_block_name.length)
            blrt_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
        blrt_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(blrt_SwmData), &node);
    }
    else if ((state & BLRT_SWM_IN_TOKEN) || (state & BLRT_SWM_TOKEN_EATEN))
    {
        // Left over token, treat as flag.
        blrt_SwmData node = blrt_SwmData_Gen();
        node.data_type = BLRT_SWM_FLAG;
        if (buffer_block_name.length)
            blrt_String_PushFrontS(&token, buffer_block_name.ptr);
        blrt_RadixTree_Insert(&(swm.contents), token.ptr, sizeof(blrt_SwmData), &node);
    }

    blrt_String_DestroyS(&token);
    blrt_String_DestroyS(&buffer_key),
    blrt_String_DestroyS(&buffer_block_name);
    return swm;
}

void
blrt_Swm_Destroy( blrt_Swm *swm )
{
    blrt_RadixTree_DestroyS(&(swm->contents));
}

bool
blrt_Swm_HasFlag( const blrt_Swm *swm, const char *flag )
{
    blrt_RadixTreeNode *node = blrt_RadixTree_Get(&(swm->contents), flag);
    if (!node || !node->data)
        return false;
    return ((blrt_SwmData *)(node->data))->data_type == BLRT_SWM_FLAG;
}

char *
blrt_Swm_GetValue( const blrt_Swm *swm, const char *key )
{
    blrt_RadixTreeNode *node = blrt_RadixTree_Get(&(swm->contents), key);
    if (!node || !node->data)
        return NULL;
    return ((blrt_SwmData *)(node->data))->data.ptr;
}
