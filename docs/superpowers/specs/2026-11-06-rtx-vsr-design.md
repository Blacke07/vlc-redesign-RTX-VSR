# RTX VSR in Performance

Approved in chat on 2026-11-06 by the author's instruction to continue.

Add one ON/OFF control, NVIDIA RTX Video Super Resolution, immediately after
Video post-processing in Performance. Reuse the existing settings row, toggle,
search, keyboard access, staging, Save/Cancel, and restart notification.

ON selects vout=direct3d11, dec-dev=d3d11_filters,
d3d11-upscale-mode=super, and removes postproc from video-filter while preserving
other filters. OFF changes only d3d11-upscale-mode to linear. The state is derived
from these VLC options; no independent saved boolean or second configuration.
The help explains that an RTX GPU, NVIDIA RTX Video enabled in the driver, and
restarting VLC are required. The switch indicates configuration, not GPU activity.

Use the v0.5.3 source and its existing Qt/C++ implementation and build scripts.
The control exists only when the Windows Direct3D11 options are available.
Keep the incumbent appearance and translations infrastructure. Add English and
Romanian text for this feature without changing other translations.

Approach: a derived setting in ConfigModel, backed by the existing configuration
transaction. A separate launcher or a second saved preset would duplicate the
configuration and is unnecessary now that the UI owns the setting.

Verify preset transitions and preservation of unrelated filters in Qt Core unit
tests; verify Save, Cancel, restart persistence, and actual VSR on the laptop.
Create Blacke07/vlc-redesign and branch feat/rtx-vsr-toggle. Build with the existing
Windows GitHub Actions workflow because Docker/WSL are absent locally. Deliver
an isolated portable test package with automatic upstream updates disabled and
an independent profile, leaving the installed player unchanged.
