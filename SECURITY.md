# Security Policy

LARVATUS es una herramienta de engaño (deception) pensada para desplegarse
únicamente en redes que administrás o donde tenés autorización explícita
para operar un honeypot.

## Reporting a Vulnerability

Si encontrás un problema de seguridad en LARVATUS mismo (no en un atacante
que interactúa con él), abrí un security advisory privado en este
repositorio o contactá al mantenedor por GitHub.

## Consideraciones de despliegue

- LARVATUS acepta conexiones de cualquier origen por diseño — es su
  propósito. No lo corras en la misma máquina donde tenés servicios reales
  de producción sin aislarlo (contenedor, VM, o segmento de red dedicado).
- Los logs (`larvatus_events.jsonl`) pueden contener direcciones IP y
  credenciales que un atacante tipeó — tratalos como datos sensibles y
  no los publiques sin sanitizar.
- El proceso no realiza ninguna acción ofensiva ni de contraataque contra
  quien se conecta.
