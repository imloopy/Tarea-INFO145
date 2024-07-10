### Compilación
Para compilar los archivos se debe:
- Descomprimir tareaSemestral.zip en una carpeta tareaSemestral.
- Ingresar a la carpeta tareaSemestral.
- Ejecutar el siguiente comando en una terminal en esta misma carpeta:

g++ src/*.cpp -I headers -o experimentacion

o en su defecto usar el comando

make

puesto que el comprimido trae un MakeFile.

### Ejecución
En el mismo directorio ejecutar el comando:
./experimentacion n f s
Siendo:
n: Tamaño del arreglo original (Lineal y normal)
f: Divisor de n para obtener el tamaño de los arreglos sample, sus tamaños serán n/f.
s: Desviación estándar para el arreglo normal.

link github: https://github.com/imloopy/Tarea-INFO145
