import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Label } from 'ng2-charts';
import { SweepService } from '../service/sweep.service';
import { Sweep } from './../sweep/sweep.component';

@Component({
  selector: 'app-device-temp',
  templateUrl: './device-temp.component.html',
  styleUrls: ['./device-temp.component.css']
})
export class DeviceTempComponent implements OnInit {

  sweeps: Sweep[] = [];
  deviceName: string = '';
  title: string = '';
  temperatureData: Array<number> = [];
  strengthData: Array<number> = [];
  timeStamp: Array<Label> = [];
  label: string = '';
  yLabel: string = '';

  constructor(private route: ActivatedRoute, 
              private service: SweepService) { }

  ngOnInit(): void {

    
    this.deviceName = this.route.snapshot.params.deviceName;
    this.title = 'Concrete Temperature'
    this.label = 'ambient temperature'
    this.yLabel = 'Temperature (Celcius)'
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
      
  }

  download(sweep: Sweep) {
    this.service.downloadSweep(sweep.id)
    .subscribe( Response => {
      saveAs(Response, sweep.filename);
    });
  }

}
