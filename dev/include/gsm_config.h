/**
 * \file            gsm_config.h
 * \brief           Configuration for GSM
 */

/*
 * Copyright (c) 2018 Tilen Majerle
 *  
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of GSM-AT.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 */
#ifndef __GSM_CONFIG_H
#define __GSM_CONFIG_H  100

/**
 * User specific config which overwrites setup from gsm_config_default.h file
 */

#if !__DOXYGEN__
#define GSM_CFG_DBG                         GSM_DBG_ON
#define GSM_CFG_DBG_TYPES_ON                GSM_DBG_TYPE_TRACE | GSM_DBG_TYPE_STATE

#define GSM_CFG_IPD_MAX_BUFF_SIZE           1460
#define GSM_CFG_INPUT_USE_PROCESS           1
#define GSM_CFG_AT_ECHO                     1

#define GSM_CFG_CONN                        1
#define GSM_CFG_SMS                         1
#define GSM_CFG_CALL                        1
#define GSM_CFG_PHONEBOOK                   1

#if defined(WIN32)
#define GSM_CFG_SYS_PORT					GSM_SYS_PORT_WIN32
#endif

#endif /* !__DOXYGEN__ */

/* Include default configuration setup */
#include "gsm/gsm_config_default.h"
 
/**
 * \}
 */

#endif /* __GSM_CONFIG_H */
