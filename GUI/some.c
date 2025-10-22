#include <gtk/gtk.h>
#include <glib.h> 

// NOTE: This code assumes the following IDs and page names exist in your XML:
// GtkWindow: 'app_window'
// GtkStack: 'main_stack'
// Page 1 (Main Menu Box) has a <packing><property name="name">page_main</property></packing>
// Page 2 (Login Box) has a <packing><property name="name">page_login</property></packing>
// Buttons: 'btn_main_login', 'btn_quit', 'btn_form_login'
// Entries: 'entry_email', 'entry_password'

// --- 1. Structure to hold pointers to critical widgets ---
typedef struct {
    GtkStack *main_stack;
    GtkEntry *entry_email;
    GtkEntry *entry_password;
    GtkWindow *app_window;
} AppWidgets;

// --- 2. Signal Handlers (The Logic) ---

// Handler for the Main Menu "Login" button (Switches to the form)
static void on_main_login_clicked(GtkButton *button, gpointer user_data) {
    AppWidgets *widgets = (AppWidgets *)user_data;
    if (widgets->main_stack) {
        // Switch to the login page using its packing name
        gtk_stack_set_visible_child_name(widgets->main_stack, "page_login");
        g_print("Action: Switched to Login Form Page.\n");
    }
}

// Handler for the Form "Login" button (Reads input)
static void on_form_login_clicked(GtkButton *button, gpointer user_data) {
    AppWidgets *widgets = (AppWidgets *)user_data;
    const char *email;
    const char *password;

    if (widgets->entry_email && widgets->entry_password) {
        
        // Reading the input text from GtkEntry widgets
        email = gtk_entry_get_text(widgets->entry_email);
        password = gtk_entry_get_text(widgets->entry_password);

        g_print("\n--- Login Attempt Data ---\n");
        g_print("Email: %s\n", email);
        g_print("Password: %s\n", password);
        g_print("--------------------------\n");
        
        // Example: Clear the fields after the attempt
        gtk_editable_set_text(GTK_EDITABLE(widgets->entry_email), "");
        gtk_editable_set_text(GTK_EDITABLE(widgets->entry_password), "");
        
    } else {
        g_warning("Error: Login entry widgets not found. Check XML object IDs.\n");
    }
}

// Handler for the "Quit" button
static void on_quit_clicked(GtkButton *button, gpointer user_data) {
    AppWidgets *widgets = (AppWidgets *)user_data;
    if (widgets->app_window) {
        gtk_window_close(widgets->app_window);
        g_print("Action: Application quit.\n");
    }
}

// --- 3. Application Setup (The Connection) ---

// Function executed when the application starts
static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *window;
    AppWidgets *g_app_widgets; 

    g_app_widgets = g_new0(AppWidgets, 1);
    
    // Load the UI file. IMPORTANT: Name must match the UI file name!
    builder = gtk_builder_new_from_file("eventmanagementsystem.ui"); 
    if (!builder) {
        g_critical("Error loading UI file 'eventmanagementsystem.ui'. Aborting.\n");
        g_free(g_app_widgets);
        return;
    }

    // Get the main window (assuming you added id="app_window" to your GtkWindow)
    window = GTK_WIDGET(gtk_builder_get_object(builder, "app_window"));
    g_app_widgets->app_window = GTK_WINDOW(window);
    gtk_window_set_application(g_app_widgets->app_window, app);

    // Get the required widgets using their object-ids
    g_app_widgets->main_stack = GTK_STACK(gtk_builder_get_object(builder, "main_stack"));
    g_app_widgets->entry_email = GTK_ENTRY(gtk_builder_get_object(builder, "entry_email"));
    g_app_widgets->entry_password = GTK_ENTRY(gtk_builder_get_object(builder, "entry_password"));

    // Connect all signal handlers defined in the UI file to the functions above
    gtk_builder_connect_signals(builder, g_app_widgets);

    gtk_window_present(GTK_WINDOW(window));
    g_object_unref(builder);
}

// Main function
int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.eventapp", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    status = g_application_run(G_APPLICATION(app), argc, argv);
    
    g_object_unref(app);
    
    return status;
}
