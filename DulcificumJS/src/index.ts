import dulcificum, { MainModule } from './Dulcificum/dulcificum_js';

let mod: MainModule | null = null;

/**
 * This must be executed and waited on before this module can be used.
 */
export async function initialize(): Promise<void> {
  if (mod !== null) {
    return;
  }
  mod = await (<any> dulcificum)({});
}

export function gcode_2_miracle_jtp(input: string): string {
  if (mod == null) {
    return "";
  }
  return <string> mod.gcode_2_miracle_jtp(input);
}

export function dulcificum_info(): dulcificum.info_t {
  if (mod == null) {
    return { dulcificum_version: "", dulcificum_hash: "" };
  }
  return mod.dulcificum_info();
}
