from dataclasses import dataclass

@dataclass
class PlatformProfile:
    raw_name: str
    defines: list[str]
    additional_compiler_flags: list[str]
    libraries: list[str]

profiles: dict[str, PlatformProfile] = {
    "PC:Linux": PlatformProfile('Linux', ['Linux'], [], []),
    "PC:Windows": PlatformProfile('Windows', ['Windows'], [], ["./scripts/windows-std-libs"])
}
