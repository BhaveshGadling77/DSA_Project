#include <gtk/gtk.h>

GtkWidget *stack;

void on_switch_to_add_clicked(GtkButton *button, gpointer user_data) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "page_add_event");
}

void on_switch_to_home_clicked(GtkButton *button, gpointer user_data) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "page_home");
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("hells.ui");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    stack = GTK_WIDGET(gtk_builder_get_object(builder, "stack_main"));

    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show_all(window);
    gtk_main();

    g_object_unref(builder);
    return 0;
}
