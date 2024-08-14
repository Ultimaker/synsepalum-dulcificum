import dulcificum, { MainModule } from './Dulcificum/dulcificum_js';

const mod: MainModule = await (<any> dulcificum)({});

export function gcode_2_miracle_jtp(input: string): string {
  return <string> mod.gcode_2_miracle_jtp(input);
}
