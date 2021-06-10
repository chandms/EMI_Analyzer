/*
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 06/10/2021
Description: A service for calling backend api for sweep data.
*/


import { HttpClient, HttpParams } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { environment as env } from 'src/environments/environment';
import { Sweep } from './sweep.component';

@Injectable({
  providedIn: 'root'
})

export class SweepService {
  

  private url = env.sweep_url;
  constructor(private http: HttpClient) { 
    
  }

  getAllSweeps(){
    return this.http.get<Sweep[]>(this.url);
  }

  getLatestSweep(){
    let params = new HttpParams;
    params = params.append('latest', 'true')
    return this.http.get<Sweep[]>(this.url, {params})
  }

}


