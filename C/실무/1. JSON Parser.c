#include <stdio.h>

struct thread_info {
    int repeat;
    int thread_num;
    char name[20];
}

void json_parser(FILE *json, thread_info *info) {
    int pointer = 0;
}

int main() {
    FILE *json = fopen("C\\실무\\build\\thread.json", "r");
    if (json == NULL) {
        perror("There is no file");
        return 0;
    }

    thread_info info;



    

    return 0;
}

// {
//     "repeat": 10,
//     "thread_num": 1,
//     "thread": [
//         {
//             "name": "sub_thread"
//         }
//     ]
// }
