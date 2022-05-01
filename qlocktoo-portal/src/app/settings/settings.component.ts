import { ClockService } from './clock.service';
import { Component, OnInit } from '@angular/core';
import { ClockConfig } from './clock.config';
import IroColorValue from '@jaames/iro';
@Component({
  selector: 'app-clock',
  templateUrl: './clock.component.html',
  styleUrls: ['./clock.component.scss']
})
export class ClockComponent implements OnInit {
  config: ClockConfig;
  colorAll: IroColorValue.Color;

  constructor(private clockService: ClockService) {
    this.config = clockService.getConfig().config;
    this.colorAll = clockService.getConfig().all;
  }

  ngOnInit(): void {
    this.submitToClock();
  }

  onColorAllChanged(): void {
    this.config.colorItIs = this.colorAll;
    this.config.colorWords = this.colorAll;
    this.config.colorHour = this.colorAll;
    this.config.colorMinutes = this.colorAll;
    this.submitToClock();
  }
  
  onColorChanged(): void {
    this.submitToClock();
  }

  private submitToClock() {
    this.clockService.saveConfig(this.config);
  }
}
