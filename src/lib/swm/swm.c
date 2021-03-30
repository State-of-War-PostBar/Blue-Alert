/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (c) 2017 - 2021 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*    State of War: Remastered is a free software. You can freely do whatever you want with it    *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                                   published by mhtvsSFrpHdE.                                   *
*                                                                                                *
*  By the time this line is written, the version of the license document is 1, but you may use   *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
*     State of War: Remastered is created, intended to be useful, but without any warranty.      *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*       You should have received a copy of the license along with the source code of this        *
*  program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.   *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                         https://github.com/State-of-War-PostBar/sowr.                          *
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#include "swm.h"

#include "../container/string.h"
#include "../memory/heap_memory.h"

static
void
sowr_FreeSwmData( void *data )
{
    sowr_String_DestroyS(&(((sowr_SwmData *)data)->data));
}

static
inline
sowr_SwmData
sowr_SwmData_Gen( void )
{
    return (sowr_SwmData)
    {
        .data = sowr_String_CreateS()
    };
}

typedef enum sowr_Swm_ParsingState
{
    /// Ready to read a token.
    SOWR_SWM_READY            = 1,
    /// Currently in a token.
    SOWR_SWM_IN_TOKEN         = 1 << 1,
    /// Ate a token and currently looking for next identifier.
    SOWR_SWM_TOKEN_EATEN      = 1 << 2,
    /// Ate a token and a = and currently looking for value for the pair.
    SOWR_SWM_AWAIT_ASSIGNMENT = 1 << 3,
    /// In comment section, everything is ignored.
    SOWR_SWM_COMMENT          = 1 << 4,
    /// A starting /, maybe a comment section later on.
    SOWR_SWM_POSSIBLE_COMMENT = 1 << 5,
    /// A [ passed through, loading block name for the block.
    SOWR_SWM_IN_BLOCK_NAME    = 1 << 6,
    /// In raw string section.
    SOWR_SWM_RAW_STRING       = 1 << 7,
    /// Invalid syntax in block name, everything is discarded.
    SOWR_SWM_DISCARD          = 1 << 8,
    /// After = and possible spaces, start assigning process.
    SOWR_SWM_ASSIGNING        = 1 << 9,
} sowr_Swm_ParsingState;

sowr_Swm
sowr_Swm_Load( const char *str )
{
    sowr_Swm swm =
    {
        .contents = sowr_RadixTree_CreateS(sowr_FreeSwmData)
    };
    if (!str)
        return swm;

    sowr_Swm_ParsingState state = SOWR_SWM_READY;
    sowr_String token = sowr_String_CreateS(),
                buffer_key = sowr_String_CreateS(),
                buffer_block_name = sowr_String_CreateS();
    sowr_String_ExpandUntilOnce(&token, 16ULL);
    sowr_String_ExpandUntilOnce(&buffer_key, 16ULL);
    sowr_String_ExpandUntilOnce(&buffer_block_name, 8ULL);
    char ch = *str;

    while (ch)
    {
        switch (ch)
        {
            case ('\r'):
            {
                // Carriage return is always ignored.
                break;
            }
            case ('\"'):
            {
                state &= ~SOWR_SWM_POSSIBLE_COMMENT;
                if (state & SOWR_SWM_COMMENT)
                    break;
                if (state & SOWR_SWM_RAW_STRING)
                {
                    // End of raw string section.
                    state &= ~SOWR_SWM_RAW_STRING;
                    break;
                }
                else
                {
                    // Start of raw string section.
                    state |= SOWR_SWM_RAW_STRING;
                    if (state & SOWR_SWM_AWAIT_ASSIGNMENT)
                    {
                        // If it's waiting for assignment, start the assignment.
                        state &= ~SOWR_SWM_AWAIT_ASSIGNMENT;
                        state |= SOWR_SWM_ASSIGNING;
                        break;
                    }
                    if (state & SOWR_SWM_TOKEN_EATEN)
                    {
                        // Raw string after eaten token and a space means that last token is a flag.
                        state &= ~SOWR_SWM_TOKEN_EATEN;
                        state |= SOWR_SWM_IN_TOKEN;
                        sowr_SwmData node = sowr_SwmData_Gen();
                        node.data_type = SOWR_SWM_FLAG;
                        if (buffer_block_name.length)
                            sowr_String_PushFrontS(&token, buffer_block_name.ptr);
                        sowr_RadixTree_Insert(&(swm.contents), token.ptr, sizeof(sowr_SwmData), &node);
                        sowr_String_Clear(&token);
                        break;
                    }
                    if (state & SOWR_SWM_IN_BLOCK_NAME)
                    {
                        // WTF? Raw string in block names?
                        // Well, it's allowed anyways as long as there are no spaces.
                        break;
                    }
                    if (state & (SOWR_SWM_IN_TOKEN | SOWR_SWM_ASSIGNING))
                    {
                        // Token conj. with raw string is allowed.
                        break;
                    }
                    if (state & SOWR_SWM_READY)
                    {
                        // Cancel ready state.
                        state &= ~SOWR_SWM_READY;
                        state |= SOWR_SWM_IN_TOKEN;
                        break;
                    }
                    break;
                }
                break;
            }
            case (' '):
            {
                state &= ~SOWR_SWM_POSSIBLE_COMMENT;
                if (state & (SOWR_SWM_COMMENT | SOWR_SWM_DISCARD))
                    break;
                if (state & SOWR_SWM_RAW_STRING)
                {
                    sowr_String_PushC(&token, ch);
                    break;
                }
                if (state & SOWR_SWM_IN_BLOCK_NAME)
                {
                    // Weird syntax like [abc.e      fdvnj]
                    // Everything after e and before ] will be discarded.
                    state |= SOWR_SWM_DISCARD;
                    break;
                }
                if (state & SOWR_SWM_AWAIT_ASSIGNMENT)
                {
                    // Space after =, ignored.
                    break;
                }
                if (state & SOWR_SWM_ASSIGNING)
                {
                    // Assignment finished, write values.
                    state = SOWR_SWM_READY;
                    break;
                }
                if (state & SOWR_SWM_IN_TOKEN)
                {
                    // Token awaits assignment.
                    state &= ~SOWR_SWM_IN_TOKEN;
                    state |= SOWR_SWM_TOKEN_EATEN;
                    break;
                }
                if (state & SOWR_SWM_READY)
                {
                    // Do nothing.
                    break;
                }
                break;
            }
            case ('\n'):
            {
                state &= ~SOWR_SWM_POSSIBLE_COMMENT;
                if (state & SOWR_SWM_RAW_STRING)
                {
                    break;
                }
                if (state & SOWR_SWM_COMMENT)
                {
                    // Stop the line of comment.
                    break;
                }
                if (state & SOWR_SWM_IN_BLOCK_NAME)
                {
                    // Weird syntax like [abc.e     
                    //                      fdvnj]
                    // Everything after e and before ] will be ignored.
                    state |= SOWR_SWM_DISCARD;
                    break;
                }
                if (state & SOWR_SWM_AWAIT_ASSIGNMENT)
                {
                    // Space after =. ignored.
                    break;
                }
                if (state & SOWR_SWM_ASSIGNING)
                {
                    // Assignment finished, write values.
                    sowr_SwmData node = sowr_SwmData_Gen();
                    node.data_type = SOWR_SWM_PAIR;
                    sowr_String_PushS(&(node.data), token.ptr);
                    if (buffer_block_name.length)
                        sowr_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                    sowr_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(sowr_SwmData), &node);

                    sowr_String_Clear(&buffer_key);
                    sowr_String_Clear(&token);
                    state = SOWR_SWM_READY;
                    break;
                }
                if (state & SOWR_SWM_IN_TOKEN)
                {
                    // Token eaten.
                    state &= ~SOWR_SWM_IN_TOKEN;
                    state |= SOWR_SWM_TOKEN_EATEN;
                    break;
                }
                if (state & SOWR_SWM_READY)
                {
                    // Do nothing.
                    break;
                }
                break;
            }
            case ('#'):
            {
                state &= ~SOWR_SWM_POSSIBLE_COMMENT;
                if (state & SOWR_SWM_COMMENT)
                    break;
                if (state & SOWR_SWM_RAW_STRING)
                {
                    sowr_String_PushC(&token, ch);
                    break;
                }
                state = SOWR_SWM_COMMENT;
                // Start comment section immediately.
                break;
            }
            case ('/'):
            {
                if (state & SOWR_SWM_COMMENT)
                    break;
                if (state & SOWR_SWM_RAW_STRING)
                {
                    sowr_String_PushC(&token, ch);
                    break;
                }
                if (state & SOWR_SWM_POSSIBLE_COMMENT)
                {
                    // If the possible comment flag is set, clear out all flags.
                    // and begin comment section.
                    state = SOWR_SWM_COMMENT;
                    break;
                }
                else
                {
                    // Otherwise, mark a possible comment flag.
                    state |= SOWR_SWM_POSSIBLE_COMMENT;
                    break;
                }
                break;
            }
            case ('['):
            {
                state &= ~SOWR_SWM_POSSIBLE_COMMENT;
                if (state & (SOWR_SWM_COMMENT | SOWR_SWM_DISCARD))
                    break;
                if (state & SOWR_SWM_RAW_STRING)
                {
                    sowr_String_PushC(&token, ch);
                    break;
                }
                if (state & (SOWR_SWM_READY | SOWR_SWM_IN_TOKEN | SOWR_SWM_TOKEN_EATEN | SOWR_SWM_AWAIT_ASSIGNMENT | SOWR_SWM_ASSIGNING))
                {
                    // Block starting. Process previous flag/assignment.
                    state = SOWR_SWM_IN_BLOCK_NAME;
                    if (state & SOWR_SWM_AWAIT_ASSIGNMENT)
                    {
                        // Block start after a = without value, treat the key as flag.
                        sowr_SwmData node = sowr_SwmData_Gen();
                        node.data_type = SOWR_SWM_FLAG;
                        if (buffer_block_name.length)
                            sowr_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                        sowr_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(sowr_SwmData), &node);
                        sowr_String_Clear(&token);
                        sowr_String_Clear(&buffer_block_name);
                        break;
                    }
                    if (state & SOWR_SWM_ASSIGNING)
                    {
                        // End current assigning.
                        sowr_SwmData node = sowr_SwmData_Gen();
                        node.data_type = SOWR_SWM_PAIR;
                        sowr_String_PushS(&(node.data), token.ptr);
                        if (buffer_block_name.length)
                            sowr_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                        sowr_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(sowr_SwmData), &node);
                        sowr_String_Clear(&buffer_key);
                        sowr_String_Clear(&token);
                        sowr_String_Clear(&buffer_block_name);
                        break;
                    }
                    if (state & (SOWR_SWM_IN_TOKEN | SOWR_SWM_TOKEN_EATEN))
                    {
                        // Block start right after previous token, treat it as flag.
                        sowr_SwmData node = sowr_SwmData_Gen();
                        node.data_type = SOWR_SWM_FLAG;
                        if (buffer_block_name.length)
                            sowr_String_PushFrontS(&token, buffer_block_name.ptr);
                        sowr_RadixTree_Insert(&(swm.contents), token.ptr, sizeof(sowr_SwmData), &node);
                        sowr_String_Clear(&token);
                        sowr_String_Clear(&buffer_block_name);
                        break;
                    }
                }
                break;
            }
            case (']'):
            {
                state &= ~SOWR_SWM_POSSIBLE_COMMENT;
                if (state & (SOWR_SWM_COMMENT))
                    break;
                if (state & SOWR_SWM_RAW_STRING)
                {
                    sowr_String_PushC(&token, ch);
                    break;
                }
                if (state & (SOWR_SWM_IN_BLOCK_NAME | SOWR_SWM_DISCARD))
                {
                    // End of block, record block name.
                    sowr_String_Clear(&buffer_block_name);
                    if (token.length)
                    {
                        sowr_String_PushS(&buffer_block_name, token.ptr);
                        sowr_String_PushC(&buffer_block_name, '.');
                    }

                    // Clear current token, and ready to record next token.
                    sowr_String_Clear(&token);
                    state = SOWR_SWM_READY;
                    break;
                }
                break;
            }
            case ('='):
            {
                state &= ~SOWR_SWM_POSSIBLE_COMMENT;
                if (state & (SOWR_SWM_COMMENT | SOWR_SWM_DISCARD))
                    break;
                if (state & SOWR_SWM_RAW_STRING)
                {
                    sowr_String_PushC(&token, ch);
                    break;
                }
                if (state & (SOWR_SWM_TOKEN_EATEN | SOWR_SWM_IN_TOKEN))
                {
                    // Token waiting for assignment, start assigning process.
                    state &= ~(SOWR_SWM_TOKEN_EATEN | SOWR_SWM_IN_TOKEN);
                    state |= SOWR_SWM_AWAIT_ASSIGNMENT;
                    sowr_String_PushS(&buffer_key, token.ptr);
                    sowr_String_Clear(&token);
                    break;
                }
                if (state & SOWR_SWM_ASSIGNING)
                {
                    // Syntax error, = sign within assignment.
                    // Assignment ends, and start record new token.
                    if (!token.length)
                    {
                        // Token is empty, treat token before = as flag.
                        sowr_SwmData node = sowr_SwmData_Gen();
                        node.data_type = SOWR_SWM_FLAG;
                        if (buffer_block_name.length)
                            sowr_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                        sowr_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(sowr_SwmData), &node);
                    }
                    else
                    {
                        sowr_SwmData node = sowr_SwmData_Gen();
                        node.data_type = SOWR_SWM_PAIR;
                        sowr_String_PushS(&(node.data), token.ptr);
                        if (buffer_block_name.length)
                            sowr_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
                        sowr_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(sowr_SwmData), &node);
                    }
                    state &= ~SOWR_SWM_ASSIGNING;
                    state |= SOWR_SWM_READY;
                    sowr_String_Clear(&buffer_key);
                    sowr_String_Clear(&token);
                    break;
                }
                if (state & SOWR_SWM_READY)
                {
                    // = without key, ignored.
                    break;
                }
                break;
            }
            case ('$'): case ('|'):
            case ('<'): case ('>'):
            case ('{'): case ('}'):
            case ('@'):
            {
                state &= ~SOWR_SWM_POSSIBLE_COMMENT;
                if (state & (SOWR_SWM_COMMENT | SOWR_SWM_DISCARD))
                    break;
                if (state & SOWR_SWM_RAW_STRING)
                {
                    sowr_String_PushC(&token, ch);
                    break;
                }
                // Reserved characters, ignored.
                break;
            }
            default:
            {
                state &= ~SOWR_SWM_POSSIBLE_COMMENT;
                if (state & (SOWR_SWM_COMMENT | SOWR_SWM_DISCARD))
                    break;
                if (state & SOWR_SWM_RAW_STRING)
                {
                    sowr_String_PushC(&token, ch);
                    break;
                }
                if (state & SOWR_SWM_AWAIT_ASSIGNMENT)
                {
                    // Start the assignment.
                    state &= ~SOWR_SWM_AWAIT_ASSIGNMENT;
                    state |= SOWR_SWM_ASSIGNING;
                    sowr_String_PushC(&token, ch);
                    break;
                }
                if (state & SOWR_SWM_TOKEN_EATEN)
                {
                    // An eaten token means that the previous token is a flag.
                    state &= ~SOWR_SWM_TOKEN_EATEN;
                    state |= SOWR_SWM_IN_TOKEN;

                    // Record the flag
                    sowr_SwmData node = sowr_SwmData_Gen();
                    node.data_type = SOWR_SWM_FLAG;
                    if (buffer_block_name.length)
                        sowr_String_PushFrontS(&token, buffer_block_name.ptr);
                    sowr_RadixTree_Insert(&(swm.contents), token.ptr, sizeof(sowr_SwmData), &node);

                    // Clear current token and start a new recording.
                    sowr_String_Clear(&token);
                    sowr_String_PushC(&token, ch);
                    break;
                }
                if (state & SOWR_SWM_READY)
                {
                    state &= ~SOWR_SWM_READY;
                    state |= SOWR_SWM_IN_TOKEN;
                    sowr_String_PushC(&token, ch);
                    break;
                }
                sowr_String_PushC(&token, ch);
                break;
            }
        }
        ch = *++str;
    }

    // Loading got shut down by EOF, evaluate left over tokens
    if (state & SOWR_SWM_RAW_STRING)
        // Unresolved raw string, discard everything.
        ;
    else if (state & SOWR_SWM_AWAIT_ASSIGNMENT)
    {
        // Left over =, treat its key as flag.
        sowr_SwmData node = sowr_SwmData_Gen();
        node.data_type = SOWR_SWM_FLAG;
        if (buffer_block_name.length)
            sowr_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
        sowr_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(sowr_SwmData), &node);
    }
    else if (state & SOWR_SWM_ASSIGNING)
    {
        // Finish the assignment.
        sowr_SwmData node = sowr_SwmData_Gen();
        node.data_type = SOWR_SWM_PAIR;
        sowr_String_PushS(&(node.data), token.ptr);
        if (buffer_block_name.length)
            sowr_String_PushFrontS(&buffer_key, buffer_block_name.ptr);
        sowr_RadixTree_Insert(&(swm.contents), buffer_key.ptr, sizeof(sowr_SwmData), &node);
    }
    else if (state & (SOWR_SWM_IN_TOKEN | SOWR_SWM_TOKEN_EATEN))
    {
        // Left over token, treat as flag.
        sowr_SwmData node = sowr_SwmData_Gen();
        node.data_type = SOWR_SWM_FLAG;
        if (buffer_block_name.length)
            sowr_String_PushFrontS(&token, buffer_block_name.ptr);
        sowr_RadixTree_Insert(&(swm.contents), token.ptr, sizeof(sowr_SwmData), &node);
    }

    sowr_String_DestroyS(&token);
    sowr_String_DestroyS(&buffer_key),
    sowr_String_DestroyS(&buffer_block_name);
    return swm;
}

void
sowr_Swm_Destroy( sowr_Swm *swm )
{
    sowr_RadixTree_DestroyS(&(swm->contents));
}

bool
sowr_Swm_HasFlag( const sowr_Swm *swm, const char *flag )
{
    sowr_RadixTreeNode *node = sowr_RadixTree_Get(&(swm->contents), flag);
    if (!node || !node->data)
        return false;
    return ((sowr_SwmData *)(node->data))->data_type == SOWR_SWM_FLAG;
}

char *
sowr_Swm_GetValue( const sowr_Swm *swm, const char *key )
{
    sowr_RadixTreeNode *node = sowr_RadixTree_Get(&(swm->contents), key);
    if (!node || !node->data)
        return NULL;
    return ((sowr_SwmData *)(node->data))->data.ptr;
}
