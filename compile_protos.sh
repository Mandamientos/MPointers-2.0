#!/bin/bash

# Configuración
PROTO_DIR="./proto"                  # Directorio de archivos .proto
OUTPUT_DIR="./gen"                   # Directorio de salida
GRPC_INSTALL_DIR="$HOME/grpc_install" # Ruta de instalación de gRPC
PROTOC_BIN="$GRPC_INSTALL_DIR/bin/protoc"  # Ruta completa a protoc
GRPC_PLUGIN="$GRPC_INSTALL_DIR/bin/grpc_cpp_plugin"  # Ruta completa al plugin

# --- Validaciones previas ---
# 1. Verificar directorio de protos
if [ ! -d "$PROTO_DIR" ]; then
    echo "Error: Directorio '$PROTO_DIR' no encontrado." >&2
    exit 1
fi

# 2. Verificar existencia de protoc
if [ ! -x "$PROTOC_BIN" ]; then
    echo "Error: $PROTOC_BIN no existe o no es ejecutable" >&2
    exit 1
fi

# 3. Verificar plugin gRPC
if [ ! -x "$GRPC_PLUGIN" ]; then
    echo "Error: Plugin gRPC no encontrado en '$GRPC_PLUGIN'" >&2
    exit 1
fi

# Crear directorio de salida (con mensaje si no existe)
mkdir -p "$OUTPUT_DIR" || {
    echo "Error: No se pudo crear el directorio de salida '$OUTPUT_DIR'" >&2
    exit 1
}

# --- Compilación ---
echo "Iniciando compilación de protos..."
echo "Usando protoc: $($PROTOC_BIN --version)"

FAILED=0
COMPILED=0

for proto_file in "$PROTO_DIR"/*.proto; do
    if [ ! -f "$proto_file" ]; then
        continue  # Por si no hay archivos .proto
    fi

    echo -n "Compilando $(basename "$proto_file")... "
    
    if "$PROTOC_BIN" --cpp_out="$OUTPUT_DIR" \
                     --grpc_out="$OUTPUT_DIR" \
                     --plugin=protoc-gen-grpc="$GRPC_PLUGIN" \
                     -I="$PROTO_DIR" \
                     "$proto_file" > /dev/null 2>&1; then
        echo "OK"
        ((COMPILED++))
    else
        echo "FALLÓ"
        ((FAILED++))
    fi
done

# --- Resultados ---
echo ""
echo "Resumen:"
echo "- Archivos compilados exitosamente: $COMPILED"
echo "- Archivos con errores: $FAILED"

if [ "$FAILED" -gt 0 ]; then
    echo "¡Advertencia! Algunos archivos fallaron." >&2
    exit 1
fi

echo "¡Compilación completada! Archivos generados en: $OUTPUT_DIR"
exit 0