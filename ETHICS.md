# Ethics

LARVATUS es una herramienta de engaño (deception) diseñada exclusivamente
para uso defensivo y autorizado.

## Uso permitido

- Desplegar LARVATUS en redes, servidores o segmentos que administrás.
- Desplegarlo con autorización explícita del dueño de la infraestructura.
- Usar los datos capturados para reforzar defensas propias (bloquear IPs
  reincidentes en tu propio firewall, entender patrones de ataque contra
  tu red).

## Uso prohibido

- Desplegar LARVATUS contra infraestructura que no controlás ni tenés
  autorización para monitorear.
- Usar las credenciales capturadas para intentar acceder a los sistemas
  reales de quien las tipeó.
- Usar los datos capturados para identificar, exponer o tomar represalias
  contra personas.

## Límite técnico intencional

LARVATUS nunca completa un protocolo real ni autentica una sesión válida.
Esto no es una limitación temporal — es una decisión de diseño para que la
herramienta nunca pueda usarse como puerta de entrada real, solo como
sensor.
