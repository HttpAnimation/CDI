#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

// Global variable for the text view widget
GtkWidget *textView;

// Function to append text to the text view widget
void append_text(const char *text) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(buffer, &iter);
    gtk_text_buffer_insert(buffer, &iter, text, -1);
}

// Function to handle Enter key press event
static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if (event->keyval == GDK_KEY_Return) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
        GtkTextIter start, end;
        gtk_text_buffer_get_bounds(buffer, &start, &end);
        gchar *command = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        // Execute the command
        FILE *fp;
        char output[1024];
        strcpy(output, "");
        fp = popen(command, "r");
        if (fp == NULL) {
            append_text("Error executing command.\n");
        } else {
            while (fgets(output, sizeof(output), fp) != NULL) {
                append_text(output);
            }
            pclose(fp);
        }

        // Append the command to the terminal log
        append_text("\n");
        append_text(command);
        append_text("\n\n");

        // Clear the text view for the next command
        gtk_text_buffer_delete(buffer, &start, &end);
        g_free(command);

        return TRUE; // Stop further processing of the event
    }
    return FALSE; // Allow further processing of the event
}

// Function to handle Ctrl+C key press event
static gboolean on_ctrl_c_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if ((event->state & GDK_CONTROL_MASK) && (event->keyval == GDK_KEY_c)) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
        gtk_text_buffer_set_text(buffer, "", -1);
        return TRUE; // Stop further processing of the event
    }
    return FALSE; // Allow further processing of the event
}

// Function to initialize the GTK application
static void activate(GtkApplication* app, gpointer user_data) {
    // Create a new window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Old CRT Terminal");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Create a text view widget for displaying the terminal output
    textView = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textView), TRUE); // Allow editing

    // Connect the key press event signals
    g_signal_connect(G_OBJECT(textView), "key-press-event", G_CALLBACK(on_key_press), NULL);
    g_signal_connect(G_OBJECT(textView), "key-press-event", G_CALLBACK(on_ctrl_c_press), NULL);

    // Create a scrolled window to contain the text view
    GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), textView);

    // Add the scrolled window to the window
    gtk_container_add(GTK_CONTAINER(window), scrolledWindow);

    // Set CSS to emulate old CRT monitor
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(cssProvider,
        "textview { background-color: #000000; color: #00FF00; font-family: monospace; }",
        -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(textView);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(cssProvider),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Show all widgets
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    // Initialize GTK application
    GtkApplication *app = gtk_application_new("org.example.CDI", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
