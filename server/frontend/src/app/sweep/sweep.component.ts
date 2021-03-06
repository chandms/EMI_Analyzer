/*
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 06/10/2021
Description: This component displays all avialable sweeps in the table.
*/

import { Component, OnInit } from '@angular/core';
import { SweepService } from '../service/sweep.service';
import { saveAs } from 'file-saver';

@Component({
  selector: 'sweep',
  templateUrl: './sweep.component.html',
  styleUrls: ['./sweep.component.css']
})
export class SweepComponent implements OnInit {
  sweeps: Sweep[] = [];
  showAllSweeps: boolean = false;
  allSweep: Sweep[] = [];
  latestSweep: Sweep[] = [];


  constructor(private service: SweepService) {}

  ngOnInit() {
    this.service.getLatestSweep()
      .subscribe(Response => {
        this.latestSweep = Response;
        this.latestSweep.forEach((sweep: Sweep) => {
          sweep.hub_timestamp = new Date(sweep.hub_timestamp);
          sweep.server_timestamp = new Date(sweep.server_timestamp);
          this.sweeps = this.latestSweep;
        });
      })

      

    this.service.getAllSweeps()    
      .subscribe(Response => {
        this.allSweep = Response;
        this.latestSweep.forEach((sweep: Sweep) => {
          sweep.hub_timestamp = new Date(sweep.hub_timestamp);
          sweep.server_timestamp = new Date(sweep.server_timestamp);
        });
      })

      
  }



  download(sweep: Sweep) {
    var file_name = sweep.device_name+"_raw.csv";
    this.service.downloadSweep(sweep.id)
    .subscribe( Response => {
      saveAs(Response, file_name);
    });
  }

  download_csv(sweep: Sweep) {
    var file_name = sweep.device_name+".csv";
    this.service.downloadSweepCsv(sweep.device_name)
    .subscribe( Response => {
      saveAs(Response, file_name);
    });
  }

  sweepDisplayChange() {
    if (this.showAllSweeps) {
      this.sweeps = this.allSweep;
    }
    else {
      this.sweeps = this.latestSweep;
    }
  }

}

export interface Sweep {
  id: number;
  device_id: number;
  device_name: string;
  sensor_timestamp: Date | any;
  hub_timestamp: Date;
  server_timestamp: Date;
  rssi: number;
  filename: string;
  strength: number;
  temperature: number | any;
  ambient_temp: number | any;

}
