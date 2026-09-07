# LARVATUS
![CMake](https://github.com/xtr4ng3-oxlgr/larvatus/actions/workflows/cmake.yml/badge.svg)

<img width="1734" height="907" alt="larvatus" src="https://github.com/user-attachments/assets/91a0d50d-be89-4009-b514-ff3a263e2c98" />


Honeypot local en C++ que emula servicios comunes (SSH, Telnet, FTP, RDP) para
detectar y registrar intentos de conexión no autorizados. No expone ningún
servicio real: cada puerto solo presenta un banner falso, captura lo que el
que se conecta intenta hacer, y cierra la conexión.

Parte de la suite OXLGR: KAHIRNET escanea tu red, VALLUM filtra el tráfico
desde adentro, LARVATUS detecta a quien igual toca la puerta.

## Por qué existe

Un firewall bien configurado te dice qué bloqueó. Un honeypot te dice quién
lo intentó, con qué credenciales, y con qué frecuencia — información que
ningún otro componente del ecosistema OXLGR captura hoy.

## Cómo funciona

- Cada servicio configurado corre en su propio hilo, escuchando en el puerto
  indicado.
- Al conectar, LARVATUS envía un banner realista del servicio simulado.
- Para SSH captura el string de identificación del cliente; para Telnet y FTP
  simula un prompt de login y captura usuario/contraseña intentados; para RDP
  registra los bytes iniciales del handshake sin interpretarlos.
- Ninguna credencial es validada nunca — el login siempre falla.
- Cada evento se escribe como una línea JSON en `larvatus_events.jsonl`.
- Un limitador por IP evita que una sola fuente sature el proceso.

## Instalación

```bash
git clone https://github.com/xtr4ng3-oxlgr/larvatus.git
cd larvatus
cmake -S . -B build
cmake --build build --config Release
```

## Uso

```bash
./build/larvatus config/larvatus.conf
```

Editá `config/larvatus.conf` para habilitar los servicios y puertos que
quieras exponer como señuelo. Los puertos estándar (22, 23, 21, 3389)
requieren privilegios de administrador/root; el archivo de ejemplo usa
puertos altos no privilegiados por defecto.

## Alcance y limitaciones

LARVATUS no implementa los protocolos reales — no negocia claves SSH, no
autentica FTP, no interpreta RDP. Es intencional: el objetivo es detectar
intención, no sostener una sesión completa.

## Licencia

MIT — ver [LICENSE](LICENSE).

---
<img width="300" height="159" alt="xtr4ng3" src="https://github.com/user-attachments/assets/3b689f22-6f4e-4ba4-bcf9-f894d9adee11" />


by xtr4ng3
