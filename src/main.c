#include "../include/app.h"

int main()
{
    app_t *app = initialize_app();
    run_app(app);
    delete_app(app);
}