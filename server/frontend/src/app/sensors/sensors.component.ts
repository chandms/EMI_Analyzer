import { Component, OnInit } from '@angular/core';
import { SweepService } from '../service/sweep.service';
import { Sweep } from './../sweep/sweep.component';
import { ActivatedRoute } from '@angular/router';
import { Label } from 'ng2-charts';

@Component({
  selector: 'app-sensors',
  templateUrl: './sensors.component.html',
  styleUrls: ['./sensors.component.css']
})
export class SensorsComponent implements OnInit {
 
  latestSweep: Sweep[] = [];
  sensors: number = 0;
  topsweeps: Sweep[] = [];


  constructor(private service: SweepService, private route: ActivatedRoute) {}

  ngOnInit() {

  	

    this.service.getLatestSweep()
      .subscribe(Response => {
        this.latestSweep = Response;
        this.sensors = this.route.snapshot.params.sensors;
        var id=0;
        while(id<this.sensors && id<this.latestSweep.length)
          	this.topsweeps.push(this.latestSweep[id++]);
      
      })
  
   


   
  }

  download(sweep: Sweep) {
    this.service.downloadSweep(sweep.id)
    .subscribe( Response => {
      saveAs(Response, sweep.filename);
    });
  }

}
