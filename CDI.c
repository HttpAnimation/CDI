#include <gtk/gtk.h>

// Simulate terminal output (replace with actual functionality)
void append_to_output(GtkWidget *textview, const gchar *text) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_get_end_iter(buffer);
    gtk_text_buffer_insert_at_cursor(buffer, text, -1);
}

gboolean on_key_press (GtkWidget *widget, GdkEventKey *event) {
    if (event->keyval == GDK_KEY_Return) {
        // Process user input (replace with actual command execution)
        gchar *input_text = gtk_entry_get_text(GTK_ENTRY(widget));
        g_print("User input: %s\n", input_text);
        append_to_output(gtk_text_view_get_parent(GTK_TEXT_VIEW(widget)), "Simulated output\n");
        gtk_entry_set_text(GTK_ENTRY(widget), "");
    }
    return TRUE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Terminal Emulator");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textview), GTK_WRAP_WORD);
    gtk_box_pack_start(GTK_BOX(vbox), textview, TRUE, TRUE, 0);

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), textview);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    GtkWidget *entry = gtk_entry_new();
    g_signal_connect(entry, "key-press-event", G_CALLBACK(on_key_press), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
