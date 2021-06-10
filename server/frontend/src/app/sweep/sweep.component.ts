/*
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 06/10/2021
Description: This component displays all avialable sweeps in the table.
*/

import { Component, OnInit } from '@angular/core';
import { SweepService } from './sweep.service';
import { saveAs } from 'file-saver';

@Component({
  selector: 'sweep',
  templateUrl: './sweep.component.html',
  styleUrls: ['./sweep.component.css']
})
export class SweepComponent implements OnInit {
  sweeps :any;

  constructor(private service: SweepService) {}

  ngOnInit() {
    this.service.getAllSweeps()
      .subscribe(Response => {
        this.sweeps = Response;
        this.sweeps.forEach((sweep: Sweep) => {
          sweep.hub_timestamp = new Date(sweep.hub_timestamp);
          sweep.server_timestamp = new Date(sweep.server_timestamp);
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

export interface Sweep {
  id: number;
  device_name: string;
  hub_timestamp: Date;
  server_timestamp: Date;
  rssi: number;
  filename: string;
}
