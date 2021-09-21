/*
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 06/22/2021
Description: This component displays all sweeps for a certain device in the table.
*/

import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Label } from 'ng2-charts';
import { SweepService } from '../service/sweep.service';
import { TrendService } from '../service/trend.service';
import { Trend } from '../trend/trend.component';
import { Sweep } from './../sweep/sweep.component';

@Component({
  selector: 'device',
  templateUrl: './device.component.html',
  styleUrls: ['./device.component.css']
})
export class DeviceComponent implements OnInit {

  sweeps: Sweep[] = [];
  trends: Trend[] = [];
  deviceName: string = '';
  title: string = '';
  temperatureData: Array<number> = [];
  strengthData: Array<number> = [];
  timeStamp: Array<Label> = [];
  label: string = '';
  yLabel: string = '';
  trendData: Array<number> = [];

  constructor(private route: ActivatedRoute, 
              private service: SweepService,
              private trendService: TrendService) { }

  ngOnInit(): void {

    this.deviceName = this.route.snapshot.params.deviceName;
    this.title = 'Concrete Strength'
    this.label = 'strength'
    this.yLabel = 'Strength (psi)'
    this.service.getDeviceSweeps(this.deviceName)
      .subscribe(Response => {
        this.sweeps = Response;
        this.sweeps.forEach(sweep => {
          console.log(sweep.ambient_temp,sweep.strength)
          this.temperatureData.push(sweep.ambient_temp);
          this.strengthData.push(sweep.strength);
          let formated_time = new Date(sweep.hub_timestamp);
          this.timeStamp.push(formated_time.toLocaleString('en-US'));
        });
      })

      this.trendService.getDeviceTrends(this.deviceName)
        .subscribe(Response => {
          this.trends = Response;
          console.log("Trends ",this.trends);
          this.trends.forEach(trend => {
            this.trendData.push(trend.trend);
          });
        })
      
  }

  download(sweep: Sweep) {
    this.service.downloadSweep(sweep.id)
    .subscribe( Response => {
      saveAs(Response, sweep.filename);
    });
  }

}

export interface Device {
  device_id: number;
  device_name: string;
  mac_address: string;
  last_updated: Date;
  longitude: number;
  latitude: number;
}
