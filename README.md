# OOP Final Project
OOP Final Project


## Notice to maintainers
After creating a schema, ensure to run:

```shell
> set PATH=%PATH%;C:\build2\bin
> odb --database sqlite --generate-query --generate-schema *.hpp, ...

OR (shorthand)

> odb -d sqlite -q -s *.hpp
```

And include the generated files (excluding any `*.ixx` files) in the VS solution for compilation.