# Contribuir

LARVATUS debe mantenerse pasivo, local y estrictamente defensivo.

Reglas:

- No implementar protocolos reales completos (sin negociación de claves SSH,
  sin autenticación FTP real, sin sesión RDP funcional).
- No agregar ningún camino donde una credencial pueda validarse como
  correcta.
- No agregar telemetría ni envío de datos capturados a servidores externos.
- No usar los datos capturados para ninguna acción automática contra la IP
  de origen (sin bloqueo automático, sin contraataque).
- Mantener el límite de conexiones por IP.
- Mantener los logs en texto plano local, nunca cifrados de forma que el
  propio operador no pueda leerlos.
- Mantener el código simple de auditar.
