// a program to organize my firefox bookmarks
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// struct definition
#define MAX_URL         512
#define MAX_TAGS        128
#define MAX_TITLE       256
#define MAX_BOOKMARKS   1000

typedef struct {
    char url[MAX_URL];
    char tags[MAX_TAGS];
    char title[MAX_TITLE];
} Bookmark;

int load_bookmarks(const char *filename, Bookmark bookmarks[], int max);
void search_bookmarks(Bookmark bookmarks[], int count, const char *keyword);
void display_bookmark(Bookmark *bm);
void list_all_bookmarks(Bookmark bookmarks[], int count);

int main(int argc, char *argv[]) {
    Bookmark bookmarks[MAX_BOOKMARKS];
    int count = load_bookmarks("bookmarks.txt", bookmarks, MAX_BOOKMARKS);
    
    if (count == 0) {
        printf("No bookmarks loaded. Check if bookmarks.txt exists.\n");
        return 1;
    }
    
    if (argc < 2) {
        // No search term provided - list everything
        list_all_bookmarks(bookmarks, count);
        return 0;
    }
    
    // Search with provided keyword
    search_bookmarks(bookmarks, count, argv[1]);
    return 0;
}

int load_bookmarks(const char *filename, Bookmark bookmarks[], int max) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 0;
    }
    
    char line[MAX_URL + MAX_TAGS + MAX_TITLE + 10];
    int count = 0;
    
    while (fgets(line, sizeof(line), fp) != NULL && count < max) {
        // remove trailing newline if present
        line[strcspn(line, "\n")] = '\0';
        
        // parse the line: URL | tags | title
        char *url = strtok(line, "|");
        char *second = strtok(NULL, "|");
        char *third = strtok(NULL, "|");
        
        // check if we got at least url and one other field
        if (url && second) {
            // trim leading spaces
            while (*url == ' ') url++;
            while (*second == ' ') second++;
            
            strncpy(bookmarks[count].url, url, MAX_URL - 1);
            bookmarks[count].url[MAX_URL - 1] = '\0';
            
            if (third) {
                // has all 3 fields: url | tags | title
                while (*third == ' ') third++;
                strncpy(bookmarks[count].tags, second, MAX_TAGS - 1);
                strncpy(bookmarks[count].title, third, MAX_TITLE - 1);
            } else {
                // only 2 fields: url | title (no tags)
                bookmarks[count].tags[0] = '\0';  // safer than strcpy
                strncpy(bookmarks[count].title, second, MAX_TITLE - 1);
            }
            bookmarks[count].tags[MAX_TAGS - 1] = '\0';
            bookmarks[count].title[MAX_TITLE - 1] = '\0';
            
            count++;
        }
    }
    
    fclose(fp);
    return count;
}

void search_bookmarks(Bookmark bookmarks[], int count, const char *keyword) {
    int found = 0;
    
    printf("\nSearching for: '%s'\n", keyword);
    printf("----------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        // search in URL, tags, title
        if (strstr(bookmarks[i].url, keyword) != NULL ||
            strstr(bookmarks[i].tags, keyword) != NULL ||
            strstr(bookmarks[i].title, keyword) != NULL) {
            
            display_bookmark(&bookmarks[i]);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No bookmarks found matching '%s'\n", keyword);
    } else {
        printf("----------------------------------------\n");
        printf("Found %d bookmark(s)\n", found);
    }
}

void list_all_bookmarks(Bookmark bookmarks[], int count) {
    printf("\nShowing all %d bookmarks:\n", count);
    printf("========================================\n");
    
    for (int i = 0; i < count; i++) {
        display_bookmark(&bookmarks[i]);
    }
    
    printf("========================================\n");
    printf("Total: %d bookmark(s)\n", count);
    printf("\nUsage: ./bookmarks <search_term> to search\n");
}

void display_bookmark(Bookmark *bm) {
    printf("\n[%s]\n", bm->title);
    printf("URL:  %s\n", bm->url);
    if (bm->tags[0] != '\0') {  // only show tags if they exist
        printf("Tags: %s\n", bm->tags);
    }
}
