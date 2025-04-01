# 🧮 Proyecto: Interpppreter

Este proyecto implementa un **intérprete interactivo de expresiones matemáticas y comandos personalizados**, escrito en C con soporte para bibliotecas dinámicas, historial de comandos y extensibilidad a través de funciones importadas.

---

## ✅ Requisitos

Antes de compilar y ejecutar, asegúrate de tener instalados los siguientes paquetes:

- **GCC** (compilador de C)
- **Make**
- **Flex** (generador de analizadores léxicos)
- **Bison** (generador de analizadores sintácticos)
- **Readline** (para entrada interactiva con historial)
- **Valgrind** (opcional, para depuración de memoria)

En sistemas basados en Debian/Ubuntu:

```bash
sudo apt install build-essential flex bison libreadline-dev valgrind
```

---

## 🗂️ Estructura del Proyecto

```
.
├── include/              # Archivos de cabecera (.h)
├── src/                  # Código fuente (.c)
├── Makefile              # Script de compilación
└── README.md             # Este archivo
```

---

## ⚙️ Uso del Makefile

### 🔸 Compilar el proyecto

```bash
make
```

Esto generará el ejecutable `ippp` en el directorio raíz.

### 🔸 Ejecutar el intérprete

```bash
./ippp
```

### 🔸 Limpiar archivos generados

```bash
make clean
```

### 🔸 Ejecutar con Valgrind (Linux)

Para comprobar fugas de memoria:

```bash
make valgrind
```

---

## 💡 Comandos disponibles en el intérprete

- `LOAD("archivo")` — Ejecuta comandos desde un archivo.
- `QUIT()` — Finaliza la ejecución o vuelve a la entrada anterior.
- `IMPORT("lib.so")` — Importa una biblioteca dinámica.
- `WORKSPACE()` — Muestra las variables, constantes y librerías cargadas.
- `CLEAR()` — Limpia todas las variables. Usa `CLEAR("FULL")` para limpiar constantes también.
- `CLEAN()` — Elimina todas las variables del entorno.
- `ECHO("ON"|"OFF")` — Activa o desactiva el eco de los resultados.
- `HELP()` — Muestra la ayuda.

> Para comandos sin argumentos, los paréntesis son opcionales.

---

## 📌 Convenciones del lenguaje

- **Variables**: deben declararse en minúsculas.
- **Constantes**: se declaran en MAYÚSCULAS y **no se pueden redefinir**.
- **Funciones**: se escriben en minúsculas (por ejemplo: `sin`, `log`).
- **Comandos**: se escriben en MAYÚSCULAS (por ejemplo: `LOAD`, `QUIT`).

---

## 🧪 Ejemplo de uso

```text
x = 5 + 3 * 2;
x
sin(PI / 2)
IMPORT("libmath.so")
WORKSPACE
```

---

## 📤 Nota

Este intérprete fue desarrollado como parte de un proyecto académico en la asignatura de Compiladores e Intérpretes.

---

🎉 ¡Listo para comenzar a interpretar comandos y jugar con expresiones matemáticas!