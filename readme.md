
# Looper de imágenes con plotter

Looper de un solo canal que carga imágenes desde `img1/` y las reproduce con framerate y dirección modulados por ruido/seno. Los ajustes se guardan automáticamente en `settings.xml`.

---

## Carpeta de imágenes

| Carpeta | Contenido |
|---------|-----------|
| `img1/` | Secuencia de frames (`.png`, `.jpg`, `.jpeg`) |

---

## Modos

| Tecla | Acción |
|-------|--------|
| `Enter` / `Backspace` | Alterna entre **modo debug** y **modo normal** |

---

## Teclas globales (cualquier modo)

### Selección de video
| Tecla | Acción |
|-------|--------|
| `0` | Deselecciona el video |
| `1` | Selecciona el video para editar su config |

### Toggle de rotaciones 90° (se guardan en XML)
| Tecla | Elemento | Variable XML |
|-------|----------|--------------|
| `2` | Video (frame de imagen) | `rotarVideo` |
| `3` | Máscara | `rotarMascara` |
| `4` | Texto | `rotarTexto` |
| `5` | Plotter | `rotarPlot` |

---

## Modo debug

### Posición del video
| Tecla | Acción |
|-------|--------|
| `W` | Sube el video |
| `S` | Baja el video |
| `A` | Mueve el video a la izquierda |
| `D` | Mueve el video a la derecha |

### Escala de imagen (independiente en X e Y)
| Tecla | Acción |
|-------|--------|
| `Q` | Reduce `scaleX` (ancho) |
| `E` | Aumenta `scaleX` (ancho) |
| `R` | Aumenta `scaleY` (alto) |
| `F` | Reduce `scaleY` (alto) |

### Parámetros de framerate (video seleccionado con `1`)
| Tecla | Acción |
|-------|--------|
| `T` / `G` | Aumenta / reduce el **bias** (centro del framerate) |
| `Y` / `H` | Aumenta / reduce la **amplitud** de variación de framerate |
| `U` / `J` | Aumenta / reduce la **velocidad** de noise |
| `I` / `K` | Aumenta / reduce el **peso** entre seno y ruido (0–1) |
| `O` / `L` | Aumenta / reduce la **velocidad de cambio de dirección** |
| `N` / `M` | Aumenta / reduce el **umbral** de cambio de dirección |
| `Z` / `X` | Aumenta / reduce la **velAcc** (velocidad del modulador de vel) |

---

## Modo normal

### Posición del texto principal
| Tecla | Acción |
|-------|--------|
| `↑` / `↓` | Mueve el texto verticalmente |
| `←` / `→` | Mueve el texto horizontalmente |
| `V` / `B` | Reduce / aumenta el gap entre líneas de texto |

### Posición del texto Mv
| Tecla | Acción |
|-------|--------|
| `W` / `S` | Mueve el texto Mv verticalmente |
| `A` / `D` | Mueve el texto Mv horizontalmente |
| `Q` / `E` | Reduce / aumenta el gap del texto Mv |

### Posición y escala del plotter
| Tecla | Acción | Paso |
|-------|--------|------|
| `I` / `K` | Mueve el plotter en X (izquierda / derecha) | 1 px |
| `J` / `L` | Mueve el plotter en Y (abajo / arriba) | 1 px |
| `N` / `M` | Aumenta / reduce el gap del plotter | 1 px |
| `R` / `F` | Aumenta / reduce el **ancho** del plotter (`sizeXPlot`, mín 50) | 5 px |
| `T` / `G` | Aumenta / reduce el **alto** del plotter (`sizeYPlot`, mín 50) | 5 px |
| `Y` / `H` | Aumenta / reduce el **historial** del plotter (`historiaPlot`, mín 4) | 5 muestras |
| `U` / `O` | Aumenta / reduce el **suavizado EMA** del plotter (0 = crudo, 1 = plano) | 0.001 |

---

## Variables XML relevantes

| Variable | Tipo | Descripción |
|----------|------|-------------|
| `scaleX` | float | Escala horizontal de la imagen |
| `scaleY` | float | Escala vertical de la imagen |
| `posX` / `posY` | float | Posición del video en pantalla |
| `posXPlot` / `posYPlot` | float | Posición del plotter |
| `sizeXPlot` / `sizeYPlot` | float | Tamaño del plotter |
| `rotarVideo` | bool | Rota el frame de imagen 90° |
| `rotarMascara` | bool | Rota la máscara 90° |
| `rotarTexto` | bool | Rota el bloque de texto 270° |
| `rotarPlot` | bool | Rota el plotter 90° alrededor de su centro |
| `direccionPlot` | bool | Dirección del eje del plotter |
| `historiaPlot` | int | Cantidad de valores en el historial del plotter |
| `bias1` | int | Centro (offset) del framerate |
| `amp1` | int | Amplitud de variación del framerate |
| `vel1` | float | Velocidad base del noise de framerate |
| `velAcc1` | float | Velocidad del modulador de velocidad |
| `peso1` | float | Mezcla seno/ruido (0 = todo ruido, 1 = todo seno) |
| `velD1` | float | Velocidad del noise de dirección |
| `umbral1` | float | Umbral de cambio de dirección (0–1) |

---

## TODO

- Versión de baja memoria: cargar solo N frames en memoria en vez del disco completo
- Versión que genere frames desde un video con la misma lógica
