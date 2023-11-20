# How to add a new object
1. create a file with the object's name
2. update `get_identifiers()` to scan for the relevant identifier
3. update malloc'd size and condition checker in `init_ids()`
4. add a relevant `init_OBJECTNAME()` function & update `init_objects()`
5. add a relevant `fill_OBJECTNAME()` function & update `fill_objects()`
6. add a relevant `check_format_OBJECTNAME()` function & add this to a `fill_OBJECTNAME()` function to the very top
7. add a relevant `free_OBJECTNAME()` function & update `free_all()`
8. update `print_contents()` for testing

