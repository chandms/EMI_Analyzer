import { HttpClient } from '@angular/common/http';
import { Component } from '@angular/core';

@Component({
  selector: 'sweep',
  templateUrl: './sweep.component.html',
  styleUrls: ['./sweep.component.css']
})
export class SweepComponent {
   sweeps :any;

  constructor(private http: HttpClient) { 
    http.get('http://10.165.76.237:50000/sweep')
      .subscribe(response => {
        this.sweeps = response;
        console.log(this.sweeps);
      })
  }

}
