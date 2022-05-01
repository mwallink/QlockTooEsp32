import { ApiService } from './../core/api-service.service';
import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Constants } from '../config/constants';
import { ClockConfig } from './clock.config';
import IroColorValue from '@jaames/iro';

@Injectable({
  providedIn: 'root'
})
export class ClockService extends ApiService {

  constructor(http: HttpClient, settings: Constants) {
    super(http, settings);
  }

  public getConfig(): { config: ClockConfig, all: IroColorValue.Color } {
    console.info('get config from Qlock');
    return {
      config: {
        colorItIs: new IroColorValue.Color('#ffffff'),
        colorWords: new IroColorValue.Color('#ffffff'),
        colorHour: new IroColorValue.Color('#ffffff'),
        colorMinutes: new IroColorValue.Color('#ffffff')
      },
      all: new IroColorValue.Color('#ffffff')
    }

    // return this.get('clock', {}).;
  }

  public saveConfig(config: ClockConfig) {
    console.info('send config to Qlock');
    console.log(config.colorItIs.hsv);

    this.post('clock', {
      colorItIs: {
        h: config.colorItIs.hsv.h!! / 360,
        // s: config.colorItIs.hsv.s!! / 100,
        s: 1,
        v: config.colorItIs.hsv.v!! / 100,
      },
      colorWords: {
        h: config.colorWords.hsv.h!! / 360,
        // s: config.colorWords.hsv.s!! / 100,
        s: 1,
        v: config.colorWords.hsv.v!! / 100,
      },
      colorHour: {
        h: config.colorHour.hsv.h!! / 360,
        // s: config.colorHour.hsv.s!! / 100,
        s: 1,
        v: config.colorHour.hsv.v!! / 100,
      }, 
      colorMinutes: {
        h: config.colorMinutes.hsv.h!! / 360,
        // s: config.colorMinutes.hsv.s!! / 100,
        s: 1,
        v: config.colorMinutes.hsv.v!! / 100,
      }
    });
  }
}
