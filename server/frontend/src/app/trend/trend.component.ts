import { Component, OnInit } from '@angular/core';
import { TrendService } from '../service/trend.service';

@Component({
  selector: 'app-trend',
  templateUrl: './trend.component.html',
  styleUrls: ['./trend.component.css']
})
export class TrendComponent implements OnInit {

  trends: Trend[] = [];
  constructor(private service: TrendService) { }

  ngOnInit(): void {
    
  }
}

export interface Trend {
  id: number;
  device_name: string;
  sensor_timestamp: Date | any;
  hub_timestamp: Date;
  server_timestamp: Date;
  rssi: number;
  filename: string;
  strength: number;
  temperature: number | any;
  ambient_temp: number | any;
  trend: number | any;

}
