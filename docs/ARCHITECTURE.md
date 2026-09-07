# Arquitectura

## Componentes

- `platform.{hpp,cpp}`: abstracción de sockets sobre Winsock/BSD sockets.
  Todo el resto del proyecto es agnóstico de sistema operativo.
- `service_profile.{hpp,cpp}`: define el banner y la interacción de cada
  servicio simulado (SSH, Telnet, FTP, RDP).
- `listener.{hpp,cpp}`: corre el bucle de aceptación de conexiones para un
  puerto, aplica el límite de conexiones por IP, y despacha al perfil de
  servicio correspondiente.
- `logger.{hpp,cpp}`: escribe cada evento capturado como una línea JSON en
  `larvatus_events.jsonl`.
- `main.cpp`: lee `config/larvatus.conf`, lanza un hilo por servicio
  habilitado, y espera indefinidamente.

## Flujo de una conexión

1. Un cliente conecta a un puerto configurado.
2. El listener verifica el límite de conexiones por IP.
3. Se despacha al perfil de servicio, que envía el banner y captura lo que
   el cliente envía (credenciales, identificación de cliente, o bytes
   crudos según el protocolo).
4. La conexión se cierra — nunca se completa un protocolo real.
5. Se registra un evento JSON con timestamp, IP/puerto de origen, servicio,
   puerto de destino, banner enviado, y datos capturados.

## Formato de log

Cada línea de `larvatus_events.jsonl` es un objeto JSON independiente:

```json
{"timestamp":"2026-09-07T14:32:10Z","source_ip":"203.0.113.5","source_port":51422,"service":"telnet","target_port":2323,"banner_sent":"...","data_received":"username=admin password=admin123"}
```
