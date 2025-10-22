#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <glib.h> // Included for g_new0 and g_free

// Structure to hold pointers to critical widgets (the stack for page switching)
typedef struct {
    GtkStack *main_stack;
} AppWidgets;

// --- Signal Handlers ---

// This handler switches the visible page in the GtkStack
static void Login(GtkWidget *widget, gpointer user_data) {
    // Cast the user_data back to our AppWidgets structure
    AppWidgets *widgets = (AppWidgets *)user_data;
    
    // Check if the stack pointer is valid before attempting to switch
    if (widgets && widgets->main_stack) {
        // Switch to the Login page using the name defined in the XML's <packing name="page_login"> tag
        gtk_stack_set_visible_child_name(widgets->main_stack, "page_login");
        g_print("Action: Switched to Login Page.\n");
    } else {
        g_warning("Error: Cannot switch page, GtkStack pointer is NULL.\n");
    }
}

// This handler switches the visible page in the GtkStack back to the main menu
static void BackToMain(GtkWidget *widget, gpointer user_data) {
    // Cast the user_data back to our AppWidgets structure
    AppWidgets *widgets = (AppWidgets *)user_data;
    
    // Check if the stack pointer is valid before attempting to switch
    if (widgets && widgets->main_stack) {
        // Switch to the Main Menu page using the name defined in the XML's <packing name="page_main"> tag
        gtk_stack_set_visible_child_name(widgets->main_stack, "page_main");
        g_print("Action: Switched back to Main Menu Page.\n");
    } else {
        g_warning("Error: Cannot switch page, GtkStack pointer is NULL.\n");
    }
}

static void Register(GtkWidget *widget, gpointer data) {
    g_print("You clicked on register button.\n");
}

// Handler to close the window
static void Quit(GtkWidget *window) {
    gtk_window_close(GTK_WINDOW(window));
}

// --- Application Setup ---

static void activate (GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    AppWidgets *g_app_widgets; 
    
    // 1. Allocate memory for the widget structure to hold the stack pointer
    g_app_widgets = g_new0(AppWidgets, 1);

    // Load the UI file. NOTE: Filename is confirmed as 'eventmanagementsystem.ui'
    gtk_builder_add_from_file(builder, "eventmanagementsystem.ui", &error);
    
    if (error) {
        // This will now explicitly print the correct filename error if the file isn't found
        g_critical("Error loading UI file 'eventmanagementsystem.ui': %s\n", error->message);
        g_error_free(error);
        g_free(g_app_widgets);
        g_object_unref(builder);
        return; 
    }

    // 2. Retrieve the main window object (ID: "window")
    GObject *window_obj = gtk_builder_get_object(builder, "window");
    
    if (!window_obj) {
        g_critical("ERROR: Could not find object with ID 'window'. UI load may have failed or ID is incorrect.\n");
        g_free(g_app_widgets);
        g_object_unref(builder);
        return; 
    }

    // 3. Retrieve the GtkStack object (ID: "main_stack") and store it in our structure
    g_app_widgets->main_stack = GTK_STACK(gtk_builder_get_object(builder, "main_stack"));
    
    // If the stack is missing, we can still run, but page switching will fail gracefully
    if (!g_app_widgets->main_stack) {
        g_warning("WARNING: Could not find GtkStack with ID 'main_stack'. Page switching will be disabled.\n");
    }
    
    GtkWindow *window = GTK_WINDOW(window_obj);
    gtk_window_set_application(window, app);

    // 4. Connect signals, passing the g_app_widgets structure as user data to the Login handler
    
    GObject *login = gtk_builder_get_object(builder, "login");
    // Passing g_app_widgets so the Login function can access the GtkStack
    if (login) g_signal_connect(login, "clicked", G_CALLBACK(Login), g_app_widgets);

    GObject *reg = gtk_builder_get_object(builder, "register");
    if (reg) g_signal_connect(reg, "clicked", G_CALLBACK(Register), NULL);

   GObject *quit = gtk_builder_get_object(builder, "quit");
    // Use connect_swapped for Quit because the handler expects the window object directly
    if (quit) g_signal_connect_swapped(quit, "clicked", G_CALLBACK(Quit), window);
    
    // --- ADD THIS CODE ---
    // 4b. Connect the "Back" button from the login page
    GObject *btn_back = gtk_builder_get_object(builder, "btn_back_to_main");
    // Passing g_app_widgets so the BackToMain function can access the GtkStack
    if (btn_back) g_signal_connect(btn_back, "clicked", G_CALLBACK(BackToMain), g_app_widgets);
    // --- END OF NEW CODE ---
    
    // 5. Set the window visible
    gtk_widget_set_visible (GTK_WIDGET (window), TRUE);

    g_object_unref (builder);
}

//By adding the new handler and connecting it to the buttos "clicked" signal, your "Back" button will now work as intended.

int main(int argc, char *argv[]) {
    // This part is standard GTK application boilerplate
    #ifdef GTK_SRCDIR
        g_chdir (GTK_SRCDIR);
    #endif
     GtkApplication *app = gtk_application_new ("org.gtk.eventapp", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
