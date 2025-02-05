#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LENGTH 7

enum Weekday {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
};

enum options {
    NEW_ALARM_LIST = 1,
    NEW_ALARM_TIME = 2,
    EDIT_ALARM = 3,
    CANCEL_ALARM = 4,
    DELETE_LIST = 5,
    DISPLAY = 6,
    EXIT = 7
};

typedef struct node {
    struct node *next;
    int alarm_time;
    int day[LENGTH];
} node;

node *newlist = NULL;

// Function prototypes
void menu();
bool createList(node **Headptr);
bool insert(node **Headptr, int alarm, int *day);
void day_string(int day);
void Display(node **Headptr);
void delete_list(node **Headptr);
void delete_alarm(node **head, int key);

// GTK Callbacks
void on_new_list_clicked(GtkButton *button, gpointer user_data);
void on_new_alarm_clicked(GtkButton *button, gpointer user_data);
void on_edit_alarm_clicked(GtkButton *button, gpointer user_data);
void on_cancel_alarm_clicked(GtkButton *button, gpointer user_data);
void on_delete_list_clicked(GtkButton *button, gpointer user_data);
void on_display_clicked(GtkButton *button, gpointer user_data);
void on_exit_clicked(GtkButton *button, gpointer user_data);

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Alarm Manager");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    button = gtk_button_new_with_label("New Alarm List");
    g_signal_connect(button, "clicked", G_CALLBACK(on_new_list_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

    button = gtk_button_new_with_label("New Alarm Time");
    g_signal_connect(button, "clicked", G_CALLBACK(on_new_alarm_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 1);

    button = gtk_button_new_with_label("Edit Alarm");
    g_signal_connect(button, "clicked", G_CALLBACK(on_edit_alarm_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 1, 1);

    button = gtk_button_new_with_label("Cancel Alarm");
    g_signal_connect(button, "clicked", G_CALLBACK(on_cancel_alarm_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 3, 1, 1);

    button = gtk_button_new_with_label("Delete List");
    g_signal_connect(button, "clicked", G_CALLBACK(on_delete_list_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 4, 1, 1);

    button = gtk_button_new_with_label("Display Alarms");
    g_signal_connect(button, "clicked", G_CALLBACK(on_display_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 5, 1, 1);

    button = gtk_button_new_with_label("Exit");
    g_signal_connect(button, "clicked", G_CALLBACK(on_exit_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 6, 1, 1);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

// GTK Callback implementations
void on_new_list_clicked(GtkButton *button, gpointer user_data) {
    if (createList(&newlist)) {
        printf("List created successfully.\n");
    } else {
        printf("Failed to create list.\n");
    }
}

void on_new_alarm_clicked(GtkButton *button, gpointer user_data) {
    int alarm;
    int day[LENGTH];

    printf("Enter alarm time (in military time): ");
    scanf("%d", &alarm);

    printf("Enter day/s of the week:\n");
    for (int i = 0; i < LENGTH; i++) {
        int day_num = -1;
        while (true) {
            printf("Is the alarm set for ");
            day_string(i);
            printf("? (0 = No, 1 = Yes): ");
            scanf("%d", &day_num);
            if (day_num == 0 || day_num == 1) {
                break;
            }
            printf("invalid input\n");
        }
        day[i] = day_num;
    }

    if (insert(&newlist, alarm, day)) {
        printf("Alarm created successfully.\n");
    } else {
        printf("Failed to create alarm.\n");
    }
}

void on_edit_alarm_clicked(GtkButton *button, gpointer user_data) {
    int alarm;
    printf("Enter alarm time to edit: ");
    scanf("%d", &alarm);
    delete_alarm(&newlist, alarm);
}

void on_cancel_alarm_clicked(GtkButton *button, gpointer user_data) {
    int alarm;
    printf("Enter alarm time to cancel: ");
    scanf("%d", &alarm);
    delete_alarm(&newlist, alarm);
}

void on_delete_list_clicked(GtkButton *button, gpointer user_data) {
    delete_list(&newlist);
    printf("List deleted successfully.\n");
}

void on_display_clicked(GtkButton *button, gpointer user_data) {
    Display(&newlist);
}

void on_exit_clicked(GtkButton *button, gpointer user_data) {
    delete_list(&newlist);
    printf("Now exiting alarm\n");
    gtk_main_quit();
}

// Original functions
bool createList(node **Headptr) {
    *Headptr = NULL;
    return true;
}

bool insert(node **Headptr, int alarm, int *day) {
    node *new_node = malloc(sizeof(node));
    if (new_node == NULL) return false;
    new_node->alarm_time = alarm;
    new_node->next = NULL;
    for (int i = 0; i < (LENGTH); i++) {
        new_node->day[i] = day[i];
    }

    if (*Headptr == NULL || alarm < (*Headptr)->alarm_time) {
        new_node->next = *Headptr;
        *Headptr = new_node;
        return true;
    }
    node *curr = *Headptr;

    while (curr->next != NULL && curr->next->alarm_time < alarm) {
        curr = curr->next;
    }

    new_node->next = curr->next;
    curr->next = new_node;
    return true;
}

void day_string(int day) {
    enum Weekday today;
    today = day;
    switch (today) {
        case MONDAY:
            printf("Monday");
            break;
        case TUESDAY:
            printf("Tuesday");
            break;
        case WEDNESDAY:
            printf("Wednesday");
            break;
        case THURSDAY:
            printf("Thursday");
            break;
        case FRIDAY:
            printf("Friday");
            break;
        case SATURDAY:
            printf("Saturday");
            break;
        case SUNDAY:
            printf("Sunday");
            break;
        default:
            printf("an unknown day");
            break;
    }
}

void Display(node **Headptr) {
    if (*Headptr == NULL) {
        printf("List is empty\n");
        return;
    }
    node *curr = *Headptr;
    int alarm_num = 1;

    while (curr != NULL) {
        printf("alarm %d is set to %d hours on ", alarm_num, curr->alarm_time);
        for (int j = 0; j < LENGTH; j++) {
            if (curr->day[j] == 1) {
                day_string(j);
                printf(" ");
            }
        }
        printf("\n");
        alarm_num++;
        curr = curr->next;
    }
}

void delete_list(node **Headptr) {
    node *curr = *Headptr;

    while (curr != NULL) {
        *Headptr = curr->next;
        free(curr);
        curr = *Headptr;
    }
}

void delete_alarm(node **head, int key) {
    node *temp = *head, *prev;

    if (temp != NULL && temp->alarm_time == key) {
        *head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->alarm_time != key) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Element not found\n");
        return;
    }

    prev->next = temp->next;
    free(temp);
}