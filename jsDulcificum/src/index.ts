import dulcificum, { MainModule } from './dulcificum.js';

const mod: MainModule = await (<any> dulcificum)({});

export function gcode_2_miracle_jtp(input: string): string {
  return <string> mod.gcode_2_miracle_jtp(input);
}
