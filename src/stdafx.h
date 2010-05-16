#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wx/wx.h>

#define EMPTY_SPACE 0
#define DISABLED_SPACE -1

#define MAX_PAGES 20
#define MAX_BITS 25

#define OP_INSERT 1
#define OP_REMOVE 2
#define OP_NONE 3

#define OP_MAX_REQUEST_LENGHT 6
#define OP_MAX_QUEUE_LENGHT 6

#define USER_MOOD_MAX 10