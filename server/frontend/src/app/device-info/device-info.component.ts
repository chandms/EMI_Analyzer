import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Label } from 'ng2-charts';
import { SweepService } from '../service/sweep.service';
import { Sweep } from './../sweep/sweep.component';

@Component({
  selector: 'app-device-info',
  templateUrl: './device-info.component.html',
  styleUrls: ['./device-info.component.css']
})
export class DeviceInfoComponent implements OnInit {

  sweeps: Sweep[] = [];
  deviceName: string = '';
  strengthData: Array<number> = [];
  timeStamp: Array<Label> = [];

  constructor(private route: ActivatedRoute, 
              private service: SweepService) { }

  ngOnInit(): void {

    
    this.deviceName = this.route.snapshot.params.deviceName;
    this.service.getDeviceSweeps(this.deviceName)
      .subscribe(Response => {
        this.sweeps = Response;
        this.sweeps.forEach(sweep => {
          this.strengthData.push(sweep.strength);
          let formated_time = new Date(sweep.hub_timestamp);
          this.timeStamp.push(formated_time.toLocaleString('en-US'));
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
