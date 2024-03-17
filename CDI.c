#include <gtk/gtk.h>

// Function to initialize the GTK application
static void activate(GtkApplication* app, gpointer user_data) {
    // Create a new window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Old CRT Terminal");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Create a text view widget for displaying the terminal output
    GtkWidget *textView = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textView), FALSE);

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
    GtkApplication *app = gtk_application_new("org.example.crt_terminal", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
