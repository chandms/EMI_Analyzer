import { HttpClient, HttpParams } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { environment as env } from 'src/environments/environment';
import { Trend } from '../trend/trend.component';

@Injectable({
  providedIn: 'root'
})
export class TrendService {

  private trend_url = env.backend_url + '/plot_data';

  constructor(private http: HttpClient) { }

  getDeviceTrends(device_name: string){
    let params = new HttpParams;
    params = params.append('device_name', device_name);
    return this.http.get<Trend[]>(this.trend_url, {params});
  }
}
