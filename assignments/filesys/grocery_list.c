#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 100
#define MAX_ITEM_LEN 50

// This program imitates a RAID(0) file system
// RAID(0) reads and writes quickly due to striping, but has no redundancy

// Mockup RAID 0 files
const char *disk_files[] = {"disk1.txt", "disk2.txt", "disk3.txt", "disk4.txt"};
const int num_disks = 4; // Number of disks

// Grocery list
char grocery_list[MAX_ITEMS][MAX_ITEM_LEN];
int item_count = 0;

// Function to add an item to the grocery list
void add_item(const char *item) {
    if (item_count < MAX_ITEMS) {
        strncpy(grocery_list[item_count], item, MAX_ITEM_LEN - 1);
        grocery_list[item_count][MAX_ITEM_LEN - 1] = '\0'; // Null-terminate
        item_count++;
    } else {
        printf("Grocery list is full.\n");
    }
}

// Function to save the grocery list to disk files with RAID 0 striping
void save_list() {
    FILE *files[num_disks];
    for (int i = 0; i < num_disks; i++) {
        files[i] = fopen(disk_files[i], "w");
        if (!files[i]) {
            perror("Error opening file");
            exit(1);
        }
    }

    // Write data to disks in a striped fashion
    for (int i = 0; i < item_count; i++) {
        for (int j = 0; j < num_disks; j++) {
            // Calculate chunk for the current disk
            int chunk_len = (strlen(grocery_list[i]) + num_disks - 1) / num_disks;
            int start_pos = j * chunk_len;
            char chunk[chunk_len + 1];
            strncpy(chunk, &grocery_list[i][start_pos], chunk_len);
            chunk[chunk_len] = '\0';

            fputs(chunk, files[j]);
            fputc('\n', files[j]);
        }
    }

    // Close all files
    for (int i = 0; i < num_disks; i++) {
        fclose(files[i]);
    }

    printf("List saved with RAID 0 mockup.\n");
}

// Function to load the grocery list from disk files in RAID 0 configuration
void load_list() {
    FILE *files[num_disks];
    for (int i = 0; i < num_disks; i++) {
        files[i] = fopen(disk_files[i], "r");
        if (!files[i]) {
            perror("Error opening file");
            exit(1);
        }
    }

    item_count = 0;
    while (1) {
        char item[MAX_ITEM_LEN] = "";

        for (int j = 0; j < num_disks; j++) {
            char chunk[MAX_ITEM_LEN / num_disks + 1];
            if (fgets(chunk, sizeof(chunk), files[j]) == NULL) break;
            chunk[strcspn(chunk, "\n")] = '\0';
            strncat(item, chunk, MAX_ITEM_LEN - strlen(item) - 1);
        }

        if (strlen(item) == 0) break;

        strncpy(grocery_list[item_count], item, MAX_ITEM_LEN - 1);
        grocery_list[item_count][MAX_ITEM_LEN - 1] = '\0'; // Null-terminate
        item_count++;
    }

    // Close all files
    for (int i = 0; i < num_disks; i++) {
        fclose(files[i]);
    }

    printf("List loaded from RAID 0 mockup.\n");
}

// Function to print the grocery list
void print_list() {
    printf("Grocery List:\n");
    for (int i = 0; i < item_count; i++) {
        printf("%d: %s\n", i + 1, grocery_list[i]);
    }
}

int main() {
    int choice;
    char item[MAX_ITEM_LEN];

    while (1) {
        printf("\nGrocery List Menu:\n");
        printf("1. Add item\n");
        printf("2. Save list\n");
        printf("3. Load list\n");
        printf("4. Print list\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1:
                printf("Enter item: ");
                fgets(item, MAX_ITEM_LEN, stdin);
                item[strcspn(item, "\n")] = '\0'; // Remove newline
                add_item(item);
                break;
            case 2:
                save_list();
                break;
            case 3:
                load_list();
                break;
            case 4:
                print_list();
                break;
            case 5:
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}