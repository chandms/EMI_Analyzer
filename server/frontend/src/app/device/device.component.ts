/*
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 06/22/2021
Description: This component displays all sweeps for a certain device in the table.
*/

import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { SweepService } from '../service/sweep.service';
import { Sweep } from './../sweep/sweep.component';

@Component({
  selector: 'device',
  templateUrl: './device.component.html',
  styleUrls: ['./device.component.css']
})
export class DeviceComponent implements OnInit {

  sweeps: Sweep[] = [];
  device_name: string = '';

  constructor(private route: ActivatedRoute, private service: SweepService) { }

  ngOnInit(): void {
    this.device_name = this.route.snapshot.params.device_name;
    this.service.getDeviceSweeps(this.device_name)
      .subscribe(Response => {
        this.sweeps = Response;
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
