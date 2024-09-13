import dulcificum, { MainModule } from './Dulcificum/dulcificum_js';

let mod: MainModule = null;

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
  return <string> mod.gcode_2_miracle_jtp(input);
}
